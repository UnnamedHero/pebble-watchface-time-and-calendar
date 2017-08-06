
var Clay = require('pebble-clay');
var clayConfig = require('./config');
var customFunctions = require('./functions');

var messageKeys = require('message_keys');
var currentLocaleModulePath = './locales/'+getLang();
var defaultLocaleModulePath = './locales/en';
try {
  var locale = require(currentLocaleModulePath);
} catch (ex) {
  var locale = require(defaultLocaleModulePath);
}

var weather = require('./weather');

function localizator(config) {
  function transaleValue(val) {
    var localizedstring = locale[val];
    return localizedstring ? localizedstring : val;
  }

  var newConfig = config.map(function(item) {
    if (item.value) {
      item.value = transaleValue(item.value);
    }
    if (item.label) {
      item.label = transaleValue(item.label);
    }
    if (item.defaultValue) {
      item.defaultValue = transaleValue(item.defaultValue);
    }
    if (item.items) {
      item.items = localizator(item.items);
    }
    if (item.options) {
      item.options = localizator(item.options);
    }
    return item;
  });
  return newConfig;
}

var clay = new Clay(localizator(clayConfig), customFunctions, { autoHandleEvents: false });

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

function getLang() {
  var countryCodes = {
    'ru-RU' : 'ru',
    'en-US' : 'en'
  }
  var lang = countryCodes[navigator.language];
  return lang ? lang : 'en';
  //return "en";
}


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
