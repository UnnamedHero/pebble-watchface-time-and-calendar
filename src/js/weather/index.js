import getProvider from './providers';
import request from '../lib/request';
import makePebbleWeatherObject from './pebbleWeatherObject';
import { messages } from './weatherAttributeTables';

export default async (options) => {
  const provider = getProvider(options.provider);
  console.log(`provider: ${provider}`);
  if (!provider) {
    return makePebbleWeatherObject(messages.weather_disabled);
  }
  const { errors, url } = await provider.makeUrl(options);

  if (errors.message) {
    console.log(`makeUrl: ${errors.message}`);
    return makePebbleWeatherObject(messages.location_error);
  }
  let weatherResponse;
  try {
    weatherResponse = await request(url);
  } catch (e) {
    return makePebbleWeatherObject(messages.unknown_error);
  }

  const weatherPOJO = JSON.parse(weatherResponse);

  const { cod } = weatherPOJO;
  if (cod === 401) {
    return makePebbleWeatherObject(messages.api_key_invalid);
  }
  if (cod === 400) {
    return makePebbleWeatherObject(messages.invalid_location_id);
  }
  if (cod !== 200) {
    return makePebbleWeatherObject(messages.unknown_error);
  }

  const weatherObj = provider.makeWeatherObj(options, weatherPOJO);
  return makePebbleWeatherObject(weatherObj, options.type);
};
