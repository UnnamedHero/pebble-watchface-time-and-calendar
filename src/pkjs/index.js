
var Clay = require('pebble-clay');
var clayConfig = require('./config');
var customFunctions = require('./functions');
var messageKeys = require('message_keys');
var i18n = require('./localizator');

var currentLocaleModulePath = './locales/' + i18n.getLang();
var defaultLocaleModulePath = './locales/en';

try {
  var locale = require(currentLocaleModulePath);
} catch (ex) {
  var locale = require(defaultLocaleModulePath);
}

var weather = require('./weather');

var clay = new Clay(i18n.localizator(clayConfig), customFunctions, { autoHandleEvents: false });

Pebble.addEventListener('showConfiguration', function(e) {
  Pebble.openURL(clay.generateUrl());
});

Pebble.addEventListener('webviewclosed', function(e) {
  if (e && !e.response) {
    return;
  }

  var dict = clay.getSettings(e.response);
  var qtb = dict[messageKeys.QuietTimeBegin].split(":",2);
  var qte = dict[messageKeys.QuietTimeEnd].split(":",2);;

  dict[messageKeys.QuietTimeBegin] = parseInt(qtb[0], 10);
  dict[messageKeys.QuietTimeBegin + 1] = parseInt(qtb[1], 10);
  dict[messageKeys.QuietTimeEnd] = parseInt(qte[0], 10);
  dict[messageKeys.QuietTimeEnd + 1] = parseInt(qte[1], 10);

  Pebble.sendAppMessage(dict, function(e) {
    console.log('Sent config data to Pebble');
  }, function(e) {
    console.log('Failed to send config data!');
  });
});

// Listen for when the watchface is opened
Pebble.addEventListener('ready',
  function(e) {
    console.log('PebbleKit JS ready!');
    Pebble.sendAppMessage({'JSReady': 1});
  }
);

Pebble.addEventListener('appmessage', function(e) {
  var dict = e.payload;
  weather.getWeather();
});
