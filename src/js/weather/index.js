import getProvider from './providers';
import request from '../lib/request';
import makePebbleWeatherObject from './pebbleWeatherObject';
import { messages } from './weatherAttributeTables';

export default async (options) => {
  const provider = getProvider(options.provider);
  if (!provider) {
    return makePebbleWeatherObject(messages.weather_disabled);
  }

  if (options.apiKey === 'not_set' || options.apiKey === '') {
    return makePebbleWeatherObject(messages.api_key_not_set);
  }

  if (options.apiKey === 'invalid') {
    return makePebbleWeatherObject(messages.api_key_invalid);
  }

  const { errors, url } = await provider.makeUrl(options);
  if (errors.message) {
    console.log(`makeUrl: ${errors.message}`);
    return makePebbleWeatherObject(messages.location_error);
  }
  let weatherResponse;
  try {
    console.log(`try to get weather data with ${url}`);
    weatherResponse = await request(url);
  } catch (e) {
    console.log(`error getting weather ${e}`);
    return makePebbleWeatherObject(messages.unknown_error);
  }
  const weatherPOJO = JSON.parse(weatherResponse);

  const code = String(weatherPOJO.cod);
  console.log(`response code ${code} at ${options.type}`);
  if (code === '401') {
    return makePebbleWeatherObject(messages.api_key_invalid);
  }
  if (code === '400') {
    return makePebbleWeatherObject(messages.invalid_location_id);
  }
  if (code !== '200') {
    return makePebbleWeatherObject(messages.unknown_error);
  }
  console.log('making weather object');
  const weatherObj = provider.makeWeatherObj(options, weatherPOJO);
  console.log(`finally! getting PEBBLE weather object ${options.type}`);
  return makePebbleWeatherObject(weatherObj, options.type);
};
