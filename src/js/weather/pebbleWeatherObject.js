import dateFns from 'date-fns';
import messageKeys from 'message_keys'; //eslint-disable-line
import { conditions, getWindDirectionSymbol, messages } from './weatherAttributeTables';
import errorHandler, { resetErrors } from './errorHandler';
import getTZOffestInSeconds from '../lib/tz-offset';

const sunStorageKey = 'SunTimes';

const isDayAt = (sunrise, sunset, time) => dateFns
  .isWithinRange(time, sunrise, sunset);

const getConditionSymbol = (conditionCode, sunrise, sunset, time = new Date()) => {
  const symbol = conditions[conditionCode].symbol || 'h';
  if (conditionCode >= 800 && conditionCode <= 803) {
    const [day, night] = symbol;
    return isDayAt(sunrise, sunset, time) ? day : night;
  }
  return symbol;
};

const getLocalTimeFromUtc = utc => new Date(utc + getTZOffestInSeconds());

const formatTime = (time, formatString) => {
  const localTime = getLocalTimeFromUtc(time);
  return dateFns.format(localTime, formatString);
};

const saveSunTimes = (sunrise, sunset) => {
  const item = { sunrise, sunset };
  localStorage.setItem(sunStorageKey, JSON.stringify(item));
};

const getSunTimes = () => JSON.parse(localStorage.getItem(sunStorageKey));

const makeWeather = (weather) => {
  const sunriseUT = weather.sunrise * 1000;
  const sunsetUT = weather.sunset * 1000;
  saveSunTimes(sunriseUT, sunsetUT);
  // console.log(`local timestamp ${getLocalTimeStamp()}`);
  return {
    WeatherMarker: true,
    WeatherTemperature: weather.temperature,
    WeatherCondition: getConditionSymbol(weather.condition, sunriseUT, sunsetUT),
    // timestamp from OWM is toooooo old
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
    const timeStamp = item.timeStamp * 1000;
    const conditionSymbol = getConditionSymbol(item.condition, sunrise, sunset, timeStamp);
    const forecastItem = {
      [messageKeys.ForecastTemperature + index]: item.temperature,
      // item.timeStamp + 60 means add one minute to timeStamp.
      // OWM return 59 min in a timestamp and it looks ugly.
      [messageKeys.ForecastTimeStamp + index]: formatTime(timeStamp + (60 * 1000), 'DD.MM HH:mm'),
      [messageKeys.ForecastCondition + index]: conditionSymbol,
    };
    return { ...acc, ...forecastItem };
  }, {});
  // console.log(`fore stamp ${forecast[0].timeStamp}`);
  return {
    WeatherMarkerForecast: true,
    ForecastQty: forecast.length,
    ForecastTime: forecast[0].timeStamp,
    ...forecastData,
    WeatherError: messages.weather_ok,
  };
};

export const makeError = (code, type) => errorHandler(code, type);

export default (data, type) => {
  const { WeatherError } = data;
  if (WeatherError) {
    return makeError(data, type);
  }
  switch (type) {
    case 'weather':
      resetErrors();
      return makeWeather(data);
    case 'forecast':
      resetErrors();
      return makeForecast(data);
    default:
      return makeError(data, type);
  }
};
