import dateFns from 'date-fns';
import messageKeys from 'message_keys'; //eslint-disable-line
import { conditions, getWindDirectionSymbol, messages } from './weatherAttributeTables';

const sunStorageKey = 'SunTimes';

const isDayAt = (sunrise, sunset, time) => dateFns
  .isWithinRange(time, new Date(sunrise), new Date(sunset));

const getConditionSymbol = (conditionCode, sunrise, sunset, time = new Date()) => {
  const symbol = conditions[conditionCode].symbol || 'h';
  if (conditionCode >= 800 && conditionCode <= 803) {
    const [day, night] = symbol;
    return isDayAt(sunrise, sunset, time) ? day : night;
  }
  return symbol;
};

const getLocalTimeFromUtc = (utc) => {
  const offset = new Date().getTimezoneOffset();
  return new Date(utc * 1000 + offset * 60);
};

const formatTime = (time, formatString) => {
  const localTime = getLocalTimeFromUtc(time);
  return dateFns.format(localTime, formatString);
};

const saveSunTimes = (sunrise, sunset) => {
  const item = {
    sunrise: sunrise * 1000,
    sunset: sunset * 1000,
  };
  localStorage.setItem(sunStorageKey, JSON.stringify(item));
};

const getSunTimes = () => JSON.parse(localStorage.getItem(sunStorageKey));

const makeWeather = (weather) => {
  const sunriseUT = weather.sunrise * 1000;
  const sunsetUT = weather.sunset * 1000;
  saveSunTimes(sunriseUT, sunsetUT);

  return {
    WeatherMarker: true,
    WeatherTemperature: weather.temperature,
    WeatherCondition: getConditionSymbol(weather.condition, sunriseUT, sunsetUT),
    // WeatherDesc: json.weather[0].description,
    WeatherTimeStamp: weather.timeStamp,
    WeatherPressure: weather.pressure,
    WeatherWindSpeed: weather.windSpeed,
    WeatherWindDirection: getWindDirectionSymbol(weather.windDirection),
    WeatherHumidity: weather.humidity,
    WeatherSunrise: formatTime(weather.sunrise, 'HH:mm'),
    WeatherSunset: formatTime(weather.sunset, 'HH:mm'),
    WeatherError: messages.weather_ok,
  };
};

const makeForecast = (forecast) => {
  const { sunrise = new Date(), sunset = new Date() } = getSunTimes();
  const forecastData = forecast.reduce((acc, item, index) => {
    const conditionSymbol = getConditionSymbol(item.condition, sunrise, sunset, item.timeStamp);
    const forecastItem = {
      [messageKeys.ForecastTemperature + index]: item.temperature,
      // item.timeStamp + 60 means add one minute to timeStamp.
      // OWM return 59 min in a timestamp and it looks ugly.
      [messageKeys.ForecastTimeStamp + index]: formatTime(item.timeStamp + 60, 'DD.MM HH:mm'),
      [messageKeys.ForecastCondition + index]: conditionSymbol,
    };
    return { ...acc, ...forecastItem };
  }, {});

  return {
    WeatherMarkerForecast: true,
    ForecastQty: forecast.length,
    ForecastTime: forecast[0].timeStamp,
    ...forecastData,
    WeatherError: messages.weather_ok,
  };
};

export const makeError = code => ({
  WeatherMarker: true,
  WeatherError: code,
});


export default (data, type) => {
  switch (type) {
    case 'weather':
      return makeWeather(data);
    case 'forecast':
      return makeForecast(data);
    default:
      return makeError(data);
  }
};
