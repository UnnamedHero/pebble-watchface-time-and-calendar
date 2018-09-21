import getProvider from './providers';
import request from '../lib/request';
import makePebbleWeatherObject from './pebbleWeatherObject';
import { messages } from './weatherAttributeTables';

// TODO: check for malformed and empty response from weather provider

export default async (options) => {
  const provider = getProvider(options.provider);
  if (!provider) {
    return makePebbleWeatherObject({ WeatherError: messages.weather_disabled });
  }

  if (options.apiKey === 'not_set' || options.apiKey === '') {
    return makePebbleWeatherObject({ WeatherError: messages.api_key_not_set });
  }

  if (options.apiKey === 'invalid') {
    return makePebbleWeatherObject({ WeatherError: messages.api_key_invalid });
  }

  const { errors, url } = await provider.makeUrl(options);
  if (errors.message) {
    console.log(`makeUrl: ${errors.message}`);
    return makePebbleWeatherObject({ WeatherError: messages.location_error }, options.type);
  }

  let weatherResponse;
  try {
    weatherResponse = await request(url);
  } catch (e) {
    return makePebbleWeatherObject({ WeatherError: messages.unknown_error }, options.type);
  }

  let weatherPOJO;
  try {
    weatherPOJO = JSON.parse(weatherResponse);
  } catch (e) {
    return makePebbleWeatherObject({ WeatherError: messages.unknown_error }, options.type);
  }

  const code = String(weatherPOJO.cod);
  if (code === '401') {
    return makePebbleWeatherObject({ WeatherError: messages.api_key_invalid });
  }
  if (code === '400') {
    return makePebbleWeatherObject({ WeatherError: messages.invalid_location_id });
  }
  if (code === '429') {
    return makePebbleWeatherObject({ WeatherError: messages.banned });
  }

  if (code !== '200') {
    return makePebbleWeatherObject({ WeatherError: messages.unknown_error }, options.type);
  }

  const weatherObj = provider.makeWeatherObj(options, weatherPOJO);
  return makePebbleWeatherObject(weatherObj, options.type);
};
