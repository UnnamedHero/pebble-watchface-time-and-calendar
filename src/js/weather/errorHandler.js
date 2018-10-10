import dateFns from 'date-fns';
import { messages } from './weatherAttributeTables';

const storageKey = 'errors';
const millisecondsInSecond = 1000;

const loadErrors = () => JSON.parse(localStorage.getItem(storageKey) || '{}');
const saveErrors = errors => localStorage.setItem(storageKey, JSON.stringify(errors));

const maxAttempts = {
  [messages.weather_ok]: 1,
  [messages.weather_disabled]: 1,
  [messages.location_error]: 8,
  [messages.invalid_location_id]: 1,
  [messages.unknown_error]: 5,
  [messages.api_key_invalid]: 1,
  [messages.api_key_not_set]: 1,
  [messages.banned]: 1,
};

const zeroErrors = {
  [messages.location_error]: {
    attempts: 0,
  },
  [messages.unknown_error]: {
    attempts: 0,
  },
};

export const resetErrors = () => {
  // console.log('reset all errors');
  saveErrors(zeroErrors);
};

const typeTable = {
  weather: {
    marker: 'WeatherMarker',
    stamp: 'WeatherTimeStamp',
    offsetInMinutes: 15,
  },
  forecast: {
    marker: 'WeatherMarkerForecast',
    stamp: 'ForecastTime',
    offsetInMinutes: 15,
  },
};

export default (pebbleObject, type) => {
  const { WeatherError } = pebbleObject;
  // console.log(`error: ${WeatherError}`);
  if (maxAttempts[WeatherError] === 1) {
    resetErrors();
    if (!type) {
      return {
        ...pebbleObject,
        WeatherMarker: true,
      };
    }
    return pebbleObject;
  }

  const allErrors = loadErrors();
  const error = allErrors[WeatherError];
  // console.log(`curr error: ${JSON.stringify(error)}`);

  const currentAttempt = error.attempts ? error.attempts + 1 : 1;
  // console.log(`error ${JSON.stringify(error)},
  // attempt ${currentAttempt}, old: ${error.attempts}, ${maxAttempts[WeatherError]}`);
  const currentType = typeTable[type];
  if (currentAttempt >= maxAttempts[WeatherError]) {
    const newTime = dateFns.addMinutes(new Date(), currentType.offsetInMinutes);
    const nextTime = Date.parse(newTime) / millisecondsInSecond;
    return {
      [currentType.marker]: true,
      [currentType.stamp]: nextTime,
      WeatherError,
    };
  }
  const newErrors = {
    ...zeroErrors,
    [WeatherError]: {
      attempts: currentAttempt,
    },
  };
  saveErrors(newErrors);
  const newTime = dateFns.addMinutes(new Date(), currentType.offsetInMinutes / 2);
  const nextTime = Date.parse(newTime) / millisecondsInSecond;
  return {
    [currentType.marker]: true,
    [currentType.stamp]: nextTime,
    WeatherError: messages.weather_ok,
  };
};
