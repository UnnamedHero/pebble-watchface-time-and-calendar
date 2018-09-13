import dateFns from 'date-fns';
import { messages } from './weatherAttributeTables';

const storageKey = 'errors';

const loadErrors = () => JSON.parse(localStorage.getItem(storageKey) || '{}');
const saveErrors = errors => localStorage.setItem(storageKey, JSON.stringify(errors));

const maxAttempts = {
  [messages.weather_ok]: 1,
  [messages.weather_disabled]: 1,
  [messages.location_error]: 5,
  [messages.invalid_location_id]: 1,
  [messages.unknown_error]: 5,
  [messages.api_key_invalid]: 1,
  [messages.api_key_not_set]: 1,
};

const zeroErrors = {
  [messages.location_error]: {
    attempts: 0,
  },
  [messages.unknown_error]: {
    attempts: 0,
  },
};

const resetErrors = () => {
  saveErrors(zeroErrors);
};

const typeTable = {
  weather: {
    marker: 'WeatherMarker',
    stamp: 'WeatherTimeStamp',
    offsetInMinutes: 60,
  },
  forecast: {
    marker: 'WeatherMarkerForecast',
    stamp: 'ForecastTime',
    offsetInMinutes: 120,
  },
};

export default (pebbleObject, type) => {
  const { WeatherError } = pebbleObject;
  console.log(`error: ${WeatherError}`);
  if (maxAttempts[WeatherError] === 1) {
    resetErrors();
    return pebbleObject;
  }

  const allErrors = loadErrors();
  const error = allErrors[WeatherError];
  console.log('try1');
  console.log(`error: ${error}`);
  console.log('try2');
  const currentAttempt = error.attempts ? error.attempts + 1 : 1;
  const currentType = typeTable[type];
  console.log('try3');
  if (currentAttempt >= maxAttempts[WeatherError]) {
    console.log('try max attempts');
    const nextTime = Date.parse(dateFns.addMinutes(new Date(), currentType.offsetInMinutes));
    return {
      [currentType.marker]: true,
      [currentType.stamp]: nextTime,
      WeatherError,
    };
  }
  console.log('try4');
  const newErrors = {
    ...zeroErrors,
    [WeatherError]: {
      attempts: currentAttempt,
    },
  };
  saveErrors(newErrors);
  const nextTime = Date.parse(dateFns.addMinutes(new Date(), currentType.offsetInMinutes / 2));
  console.log('return!');
  return {
    [currentType.marker]: true,
    [currentType.stamp]: nextTime,
    WeatherError: messages.weather_ok,
  };
};
