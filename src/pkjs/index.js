
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

function getWeatherAPIKey() {
  return localStorage.getItem('clay-settings') ?
   JSON.parse(localStorage.getItem('clay-settings')).WeatherAPIKey :
   "not_set";
}

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos) {
  // We will request the weather here
  var weatherAPIKey = getWeatherAPIKey();
//  return;
   if (weatherAPIKey == "not_set" || weatherAPIKey == "invalid_api_key" ) {
     console.log("ERROR: Weather API key is not set.");
     return;
   }

  var url = 'http://api.openweathermap.org/data/2.5/weather?lat=' +
      pos.coords.latitude +
      '&lon=' + pos.coords.longitude +
      '&lang=' + getLang() +
      '&appid=' + weatherAPIKey;// + 'ru';
  console.log(url);

  xhrRequest(url, 'GET',
    function(responseText) {
    // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);
    // Temperature in Kelvin requires adjustment
      console.log(responseText);
      if (json.cod === 401) {
        console.log (json.message);
        var dict = {
          "ConfigMarker": true,
          "WeatherAPIKey": "invalid_api_key"
        };
        Pebble.sendAppMessage(dict);
        return;
      }
      console.log (json.message);
      var temperature = Math.round(json.main.temp - 273.15);

      var weather = {
        "WeatherMarker": true,
        "WeatherTemperature": temperature,
        "WeatherDesc": json.weather[0].description,
        "WeatherTimeStamp": json.dt,
        "WeatherPressure": json.main.pressure,
        "WeatherWindSpeed": json.wind.speed,
        "WeatherWindDirection": json.wind.deg
      };
      Pebble.sendAppMessage(weather, function(e) {
        console.log('Weather info sent to Pebble successfully!');
      }, function(e) {
        console.log('Error sending weather info to Pebble!');
      });
    }
  );
}

function locationError(err) {
  console.log('Error requesting location!');
}

function fakeWeather() {
//  console.log("Api: " + weatherAPIKey);
  var json = JSON.parse('{"coord":{"lon":37.62,"lat":55.75},"weather":[{"id":802,"main":"Clouds","description":"scattered clouds","icon":"03d"}],"base":"stations","main":{"temp":287.4,"pressure":1007,"humidity":82,"temp_min":287.15,"temp_max":288.15},"visibility":10000,"wind":{"speed":7,"deg":270},"clouds":{"all":40},"dt":1499612400,"sys":{"type":1,"id":7323,"message":0.002,"country":"RU","sunrise":1499561928,"sunset":1499623808},"id":524901,"name":"Moscow","cod":200}');
  var temperature = Math.round(json.main.temp - 273.15);
  var weather = {
    "WeatherMarker": true,
    "WeatherTemperature": temperature,
    "WeatherDesc": json.weather[0].description,
    "WeatherHumidity": json.main.humidity,
    "WeatherPressure": json.main.pressure,
    "WeatherWindSpeed": json.wind.speed,
    "WeatherWindDirection": json.wind.deg
  };
  Pebble.sendAppMessage(weather, function(e) {
    console.log('Weather info sent to Pebble successfully!');
  }, function(data, err) {
      console.log('Error sending weather info to Pebble!, '+err);
    });
}

function getWeather() {
  //fakeWeather();
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 5000, maximumAge: 0}
  );

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
  getWeather();
});
