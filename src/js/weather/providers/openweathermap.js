import getGeoPosition from '../../lib/geoposition';

const getLocation = async (options) => {
  const errors = {};
  if (options.locationType === 'cid') {
    if (options.cityId === 'not_set' || options.cityId === 'invalid_id') {
      errors.message = 'City ID Error';
      return { errors };
    }
    const location = `?id=${options.cityId}`;
    return { errors, location };
  }

  try {
    const pos = await getGeoPosition();
    const location = `?lat=${pos.coords.latitude}&lon=${pos.coords.longitude}`;
    return { errors, location };
  } catch (e) {
    errors.message = 'GPS Location Error';
    return { errors };
  }
};

const getTemperatureUnits = (unitsKey) => {
  switch (unitsKey) {
    case 'imperial':
      return '&units=imperial';

    case 'metric':
      return '&units=metric';

    case 'default':
      return '';

    default:
      return '';
  }
};

export const makeUrl = async (options) => {
  const addr = 'http://api.openweathermap.org/data/2.5/';
  const requestType = options.type;
  const { errors, location } = await getLocation(options);
  // console.log(`location errer: ${errors.message}`);
  const temperatureUnits = getTemperatureUnits(options.units);
  const key = `&appid=${options.apiKey}`;

  const urlParts = [addr, requestType, location, temperatureUnits, key];
  return { errors, url: urlParts.join('') };
};

const makeWeather = weather => ({
  temperature: Math.round(weather.main.temp),
  condition: weather.weather[0].id,
  // desc: weather.weather[0].description,
  timeStamp: weather.dt,
  pressure: Math.round(weather.main.pressure * 0.75) - 14,
  windSpeed: weather.wind.speed,
  windDirection: weather.wind.deg,
  humidity: weather.main.humidity,
  sunrise: weather.sys.sunrise,
  sunset: weather.sys.sunset,
});


const getForecastItems = (forecastType) => {
  switch (forecastType) {
    case 'ft_off':
      return [];
    case 'ft_3h':
      return [0, 1, 2, 3];
    case 'ft_6h':
      return [0, 2, 4, 6];
    default:
      return [0, 1, 2, 3];
  }
};

const makeForecast = (weather, forecastType) => {
  const forecastItems = new Set(getForecastItems(forecastType));
  const forecast = weather.list
    .filter((_, index) => forecastItems.has(index))
    .map(item => ({
      timeStamp: item.dt,
      temperature: Math.round(item.main.temp),
      condition: item.weather[0].id,
    }));
  return forecast;
};

export const makeWeatherObj = (options, weatherResponse) => {
  if (options.type === 'forecast') {
    return makeForecast(weatherResponse, options.forecastType);
  }
  return makeWeather(weatherResponse);
};
