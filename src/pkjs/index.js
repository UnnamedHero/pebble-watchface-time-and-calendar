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

var countryCodes = {
  'ru_RU' : 'ru',
  'en_US' : 'en'
}
var weatherAPIKey = "ad68120f127506277ac967b76c4ae687";
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

function getLang() {
  var lang = countryCodes[navigator.language];
  return lang ? lang : 'en';
}

function locationSuccess(pos) {
  // We will request the weather here
  // var apiKey = clay.getItemById('WeatherAPIKey').get();
  // console.log(apiKey);
  weatherAPIKey = "ad68120f127506277ac967b76c4ae687";
  return;
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
      console.log('Temperature is ' + temperature);
    // Conditions
      var conditions = json.weather[0].main;
      console.log('Conditions are ' + conditions);

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
function fakeWeather() {
  var json = JSON.parse('{"coord":{"lon":37.62,"lat":55.75},"weather":[{"id":802,"main":"Clouds","description":"вот такое лето","icon":"03d"}],"base":"stations","main":{"temp":287.4,"pressure":1007,"humidity":82,"temp_min":287.15,"temp_max":288.15},"visibility":10000,"wind":{"speed":7,"deg":270},"clouds":{"all":40},"dt":1499612400,"sys":{"type":1,"id":7323,"message":0.002,"country":"RU","sunrise":1499561928,"sunset":1499623808},"id":524901,"name":"Moscow","cod":200}');
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
  }, function(e) {
      console.log('Error sending weather info to Pebble!');
    });



}

function getWeather() {

  fakeWeather();
  // navigator.geolocation.getCurrentPosition(
  //   locationSuccess,
  //   locationError,
  //   {timeout: 5000, maximumAge: 0}
  // );

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
  // if (dict['WeatherAPIKey']) {
  //   weatherAPIKey = dict['WeatherAPIKey'];
  //   console.log("Received Weather API Key:", weatherAPIKey);
  //   getWeather();
  // }
});
