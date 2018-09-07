import * as openWeatherMap from './openweathermap';

const providers = {
  disabled: null,
  OWM: openWeatherMap,
};

export default providerKey => providers[providerKey];
