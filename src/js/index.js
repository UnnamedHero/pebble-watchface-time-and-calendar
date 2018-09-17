
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

  const fixTimeField = fields => fields.forEach((field) => {
    if (dict[field]) {
      const timeValue = dict[field].split(':', 2);
      dict[field] = parseInt(timeValue[0], 10);
      dict[field + 1] = parseInt(timeValue[1], 10);
    }
  });

  fixTimeField(
    [
      messageKeys.QuietTimeBegin,
      messageKeys.QuietTimeEnd,
      messageKeys.ColorShiftTimeBegin,
      messageKeys.ColorShiftTimeEnd,
    ],
  );

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
    provider: dict[messageKeys.WeatherProvider],
    apiKey: dict[messageKeys.WeatherAPIKey],
    cityId: dict[messageKeys.NP_CityID],
    locationType: dict[messageKeys.NP_WeatherLocationType],
    units: dict[messageKeys.WeatherUnits],
    forecastType: dict[messageKeys.ForecastType],
  };

  localStorage.setItem('clay-helper', JSON.stringify(helperSettings));
  sendToPebble(dict);
});

Pebble.addEventListener('ready', () => { //eslint-disable-line
  console.log('PebbleKit JS ready!');
  sendToPebble({ JSReady: 1 });
});

Pebble.addEventListener('appmessage', async (e) => { //eslint-disable-line
  const message = e.payload;
  const isForecastRequest = message.WeatherMarkerForecast === 1;
  const data = JSON.parse(localStorage.getItem('clay-helper') || '{}');
  const options = {
    ...data,
    type: isForecastRequest ? 'forecast' : 'weather',
  };
  const pebbleWeather = await getWeather(options);
  sendToPebble(pebbleWeather);
});
