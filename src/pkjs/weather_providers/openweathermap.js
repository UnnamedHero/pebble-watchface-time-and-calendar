var i18n = require('./../localizator');

function getWeatherAPIKey() {
	return localStorage.getItem('clay-settings') ?
		JSON.parse(localStorage.getItem('clay-settings')).WeatherAPIKey :
		"not_set";
	}

function getTempUnits() {
  var units = JSON.parse(localStorage.getItem('clay-settings')).WeatherUnits;
  switch (units) {
    case 'imperial':
      return '&units=imperial';
    case 'metric':
      return '&units=metric';
    case 'default':
      return '';
    default:
      return '';
  }
}

function xhrRequest (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
}

function sendWeather(weather_data) {
    Pebble.sendAppMessage(weather_data, function(e) {
      console.log('Weather info sent to Pebble successfully!');
    }, function(e) {
      console.log('Error sending weather info to Pebble!');
    });  
}

function isDayNow (sunrise, sunset) {
  var now = Date.now();
  return now > sunrise && now < sunset;
}

//http://openweathermap.org/weather-conditions
function getCondition (owmCond, sunrise, sunset) {

  var isDay = isDayNow(sunrise, sunset);
//Thunderstorm
  switch (owmCond) {
//    
    case 200:
    case 201:
    case 202:
      return '`';
    case 210:
    case 211:
    case 212:
    case 221:
      return 'F';
    case 230:
    case 231:
    case 232: 
      return '_';
//Drizzle
    case 300:
    case 301:
    case 302:
    case 310:
    case 311:
    case 312:
    case 313:
    case 314:
    case 321:
      return "'";
  // light rain
    case 500: 
      return "6";
  //moderate rain
    case 501:
      return '$';
  //heavy rain
    case 502:
    case 503:
    case 504:
      return 'a';
    case 511:
      return 's';    
    case 520:
    case 521:
    case 522:
    case 531:
      return '*';
    case 600:
    case 601:
    case 602:
    case 620:
    case 621:
    case 622:
      return 't';
    case 611:
    case 612:
    case 615:
    case 616:
      return 'u'; 
    case 700:
    case 741:
      return '?';
    case 711:
      return 'А';
    case 721:
      return '<';
    case 731:
    case 751:
      return '<';
    case 761:
      return 'Б';
//clear sky      
    case 800:
      return isDay ? 'I' : 'N';
    case 801:
      return isDay ? 'b' : '#';
    case 802:
      return isDay ? 'c' : '#';
    case 803:
      return isDay ? '"' : '#';
    case 804:
      return '!';
    default:
      return 'h';
  }
}

function locationSuccess(pos) {
  // We will request the weather here
  var weatherAPIKey = getWeatherAPIKey();
//  return;
   if (weatherAPIKey == "not_set" || weatherAPIKey == "invalid_api_key" ) {
     console.log("Waether ERROR: Invalid API key");
     return ;
   }

  var url = 'http://api.openweathermap.org/data/2.5/weather?lat=' +
      pos.coords.latitude +
      '&lon=' + pos.coords.longitude +
      '&lang=' + i18n.getLang() +
      getTempUnits() + 
      '&appid=' + weatherAPIKey;// + 'ru';
  
  return xhrRequest(url, 'GET',
    function(responseText) {
  //    fetchingWeather = false;
    // responseText contains a JSON object with weather info
    	var json = JSON.parse(responseText);

    	if (json.cod === 401) {
    		console.log("Waether ERROR: Invalid API key");
        return;
    	}
		//var temperature = Math.round(json.main.temp - 273.15);
    // Temperature in Kelvin requires adjustment
    
      	sendWeather ({
        "WeatherMarker": true,
        "WeatherTemperature": json.main.temp,
        "WeatherCondition": getCondition(json.weather[0].id, json.sys.sunrise, json.sys.sunset),
//        "WeatherDesc": json.weather[0].description,
        "WeatherTimeStamp": json.dt,
        // "WeatherPressure": json.main.pressure,
        // "WeatherWindSpeed": json.wind.speed,
        // "WeatherWindDirection": json.wind.deg
      	});

    }
  );
}

function locationError(err) {  
  return {
  	"WeatherError":"Error requesting location!"
  }
}



exports.getWeather = function() {

return navigator.geolocation.getCurrentPosition(
	locationSuccess,
	locationError,
	{timeout: 5000, maximumAge: 0}
	);

	// var weatherAPIKey = getWeatherAPIKey();


	// var url = 'http://api.openweathermap.org/data/2.5/weather?lat=' +
 //       		pos.coords.latitude +
 //       		'&lon=' + pos.coords.longitude +
 //       		'&lang=' + getLang() +
 //       		'&appid=' + weatherAPIKey;
	// };





//-------


// function sendWeather(weather_data) {
//     Pebble.sendAppMessage(weather_data, function(e) {
//       console.log('Weather info sent to Pebble successfully!');
//     }, function(e) {
//       console.log('Error sending weather info to Pebble!');
//     });	
// }

// function getWeather() {
//   var weather_data = provider.fakeWeather();
//   sendWeather(weather_data);
//   if (!fetchingWeather) {
//   	  fetchingWeather = true;
//   }	



};