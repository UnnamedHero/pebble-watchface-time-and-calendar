// Import the Clay package
var Clay = require('pebble-clay');
// Load our Clay configuration file
var clayConfig = require('./config');
var customFunctions = require('./functions');
// Initialize Clay
var clay = new Clay(clayConfig, customFunctions);
// var clay = new Clay(clayConfig, customFunctions, {
//   locales: {
//     en_US: require('./locale/config/en-us.json'),
//     ru_RU: require('./locale/config/ru-ru.json'),
//   },
//   bundleAllLocales: true
// });
var messageKeys = require('message_keys');

var weatherAPIKey = "not_set";
// var settings = clay.getSettings();
// console.log(settings[messageKey.WeatherAPIKey]);

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
  // var apiKey = clay.getItemById('WeatherAPIKey').get();
  // console.log(apiKey);
   if (weatherAPIKey == "not_set" ) {
     console.log("ERROR: Weather API key is not set.");
     return;
   }

  var url = 'http://api.openweathermap.org/data/2.5/weather?lat=' +
      pos.coords.latitude + '&lon=' + pos.coords.longitude + '&appid=' + weatherAPIKey;

  xhrRequest(url, 'GET',
    function(responseText) {
    // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);
    // Temperature in Kelvin requires adjustment
//    console.log(responseText);
      var temperature = Math.round(json.main.temp - 273.15);
      console.log('Temperature is ' + temperature);
    // Conditions
      var conditions = json.weather[0].main;
      console.log('Conditions are ' + conditions);

      var weather = {
        "WeatherMarker": true,
        "WeatherTemperature": temperature,
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

// function getWeatherAPIKey() {
//   var request = {
//     "WeatherAPIKeyGetter": true
//   };
//   Pebble.
// }

function getWeather() {
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
    // Get the initial weather
    //getWeather();
  }
);

Pebble.addEventListener('appmessage', function(e) {
  var dict = e.payload;
  if (dict['WeatherAPIKey']) {
    weatherAPIKey = dict['WeatherAPIKey'];
    console.log("REceived Weather API Key:", weatherAPIKey);
    getWeather();
  }  
  if (dict['WeatherMarker']) {
    getWeather();
  }

});
