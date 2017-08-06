module.exports = function () {

var fetchingWeather = false;

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
      fetchingWeather = false;
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
  fetchingWeather = false;
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
  fakeWeather();
//   if (!fetchingWeather) {
//   	  fetchingWeather = true;
// 	  navigator.geolocation.getCurrentPosition(
// 	    locationSuccess,
// 	    locationError,
// 	    {timeout: 5000, maximumAge: 0}
// 	  );
//   }	
// }

};