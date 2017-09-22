
var Clay = require('pebble-clay');
var clayConfig = require('./config');
var customFunctions = require('./functions');
var messageKeys = require('message_keys');
var i18n = require('./localizator');
var sender = require('./sender.js');
var clay = new Clay(i18n.translate(clayConfig), customFunctions, { autoHandleEvents: false });

Pebble.addEventListener('showConfiguration', function(e) {
  Pebble.openURL(clay.generateUrl());
});

Pebble.addEventListener('webviewclosed', function(e) {
  if (e && !e.response) {
    return;
  }
  var dict = clay.getSettings(e.response);

  var watch = Pebble.getActiveWatchInfo();
  if (watch.platform === 'aplite') {  
    var qtb = dict[messageKeys.QuietTimeBegin].split(":",2);
    var qte = dict[messageKeys.QuietTimeEnd].split(":",2);
    dict[messageKeys.QuietTimeBegin] = parseInt(qtb[0], 10);
    dict[messageKeys.QuietTimeBegin + 1] = parseInt(qtb[1], 10);
    dict[messageKeys.QuietTimeEnd] = parseInt(qte[0], 10);
    dict[messageKeys.QuietTimeEnd + 1] = parseInt(qte[1], 10);
  }
  var dateFormat = dict[messageKeys.DateFormat];
  var dateSeparator = dict[messageKeys.DateFormatSeparator];
  var newDateFormat = dateFormat.replace(new RegExp ('\\.','g'), dateSeparator);
  dict[messageKeys.DateFormat] = newDateFormat;
  dict[messageKeys.ConfigMarker] = true;
  var sw_timeout_str = dict[messageKeys.SwitchBackTimeout];
  dict[messageKeys.SwitchBackTimeout] = parseInt(sw_timeout_str, 10);
  
  var mySettings = {
    'WeatherAPIKey' : dict[messageKeys.WeatherAPIKey]
  }
  
  localStorage.setItem('clay-helper', JSON.stringify(mySettings));
  sender.send(dict);
});


Pebble.addEventListener('ready',
  function(e) {
    console.log('PebbleKit JS ready!');
    sender.send({'JSReady': 1});
  }
);

Pebble.addEventListener('appmessage', function(e) {

  var dict = e.payload;
//  var provider = require('./weather_providers/dummy');  
  var provider = require('./weather_providers/openweathermap');
  if (dict.WeatherMarkerForecast === 1) {
    console.log("Go for a forecast");
    provider.getWeather('forecast');
    return;
  }
  if (dict.WeatherMarker === 1) {
    console.log("Go for a weather");
    provider.getWeather();
    return;
  }
});
