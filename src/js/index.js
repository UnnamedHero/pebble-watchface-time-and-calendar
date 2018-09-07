
import Clay from 'pebble-clay';
import messageKeys from 'message_keys'; //eslint-disable-line
import clayConfig from './config';
import customFunctions from './functions';
import localizator from './localizator';
import sendToPebble from './sender';
import getWeather from './weather';

const clay = new Clay(localizator(clayConfig), customFunctions, { autoHandleEvents: false });

Pebble.addEventListener('showConfiguration', () => { //eslint-disable-line
  Pebble.openURL(clay.generateUrl()); //eslint-disable-line
});

Pebble.addEventListener('webviewclosed', (e) => { //eslint-disable-line
  if (e && !e.response) {
    return;
  }

  const dict = clay.getSettings(e.response);

  const watch = Pebble.getActiveWatchInfo(); //eslint-disable-line
  if (watch.platform === 'aplite') {
    const qtb = dict[messageKeys.QuietTimeBegin].split(':', 2);
    const qte = dict[messageKeys.QuietTimeEnd].split(':', 2);
    dict[messageKeys.QuietTimeBegin] = parseInt(qtb[0], 10);
    dict[messageKeys.QuietTimeBegin + 1] = parseInt(qtb[1], 10);
    dict[messageKeys.QuietTimeEnd] = parseInt(qte[0], 10);
    dict[messageKeys.QuietTimeEnd + 1] = parseInt(qte[1], 10);
  }
  const dateFormat = dict[messageKeys.DateFormat];
  const dateSeparator = dict[messageKeys.DateFormatSeparator];
  const newDateFormat = dateFormat.replace(new RegExp('\\.', 'g'), dateSeparator);
  dict[messageKeys.DateFormat] = newDateFormat;
  dict[messageKeys.ConfigMarker] = true;

  const switchBackTimeout = dict[messageKeys.SwitchBackTimeout];
  dict[messageKeys.SwitchBackTimeout] = parseInt(switchBackTimeout, 10);

  const switchBackTimeoutSeconds = dict[messageKeys.SwitchBackTimeoutSeconds];
  dict[messageKeys.SwitchBackTimeoutSeconds] = parseInt(switchBackTimeoutSeconds, 10);

  const pebbleShakeAction = dict[messageKeys.PebbleShakeAction];
  dict[messageKeys.PebbleShakeAction] = parseInt(pebbleShakeAction, 10);

  const helperSettings = {
    WeatherAPIKey: dict[messageKeys.WeatherAPIKey],
    NP_CityID: dict[messageKeys.NP_CityID],
    NP_WeatherLocationType: dict[messageKeys.NP_WeatherLocationType],
  };

  localStorage.setItem('clay-helper', JSON.stringify(helperSettings));
  sendToPebble(dict);
});

Pebble.addEventListener('ready', () => { //eslint-disable-line
  console.log('PebbleKit JS ready!');
  sendToPebble({ JSReady: 1 });
});

const makeWeatherOptions = (isForecast) => {
  const claySettings = JSON.parse(localStorage.getItem('clay-settings') || '{}');
  return {
    // TODO: dev
    // provider: claySettings.WeatherProvider,
    provider: 'OWM',
    apiKey: claySettings.WeatherAPIKey || 'not_set',
    locationType: claySettings.NP_WeatherLocationType,
    cityId: claySettings.NP_CityID || 'not_set',
    units: claySettings.WeatherUnits,
    type: isForecast ? 'forecast' : 'weather',
    forecastType: claySettings.ForecastType,
  };
};

Pebble.addEventListener('appmessage', async (e) => { //eslint-disable-line
  const message = e.payload;
  const isForecastRequest = message.WeatherMarkerForecast === 1;
  const options = makeWeatherOptions(isForecastRequest);

  const pebbleWeather = await getWeather(options);
  sendToPebble(pebbleWeather);
});
