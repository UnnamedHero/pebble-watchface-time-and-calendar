var i18n = require('./../localizator');
var sender = require('./../sender');
var messageKeys = require('message_keys');
isForecast = false;

exports.getWeather = function(type) {  
  isForecast = false;

  var success = type === 'forecast' ? locationForecast : locationSuccess;  

  return navigator.geolocation.getCurrentPosition(
    success,
    locationError,
    {timeout: 5000, maximumAge: 0}
    );
};


function getWeatherAPIKey() {
 var helperSettings = localStorage.getItem('clay-helper');
 var claySettings = localStorage.getItem('clay-settings');
 var weatherAPI = claySettings ? JSON.parse(claySettings).WeatherAPIKey : 'not_set';

 if (!helperSettings) {
   clayHelperSetItem('WeatherAPIKey', weatherAPI);
   return weatherAPI;
  }   
  return JSON.parse(helperSettings).WeatherAPIKey;   
}

function clayHelperSetItem(key, value) {
  var settings = JSON.parse(localStorage.getItem('clay-helper')) || {} ; 

  if (key) {
    settings[key] = value;
  }
  localStorage.setItem('clay-helper', JSON.stringify(settings));
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
    sender.send(weather_data);
}

function isDayAt (sunrise, sunset, time) {
  if(!sunrise) {
    var sunData = JSON.parse(localStorage.getItem('SunTimes'));
    if (!(sunData && sunData.sunrise && sunData.sunset)) {
      return true;
    } 
    sunrise = sunData.sunrise;
    sunset = sunData.sunset;        
  }
  var newSunrise = minutesOfADay(sunrise);
  var newSunset = minutesOfADay(sunset);  
  var now = time ? 
    minutesOfADay(timeFromUtc(time).getTime()) :
    minutesOfADay(Date.now()); 
  return now > newSunrise && now < newSunset;
}

function getWindDirection(direction) {
  var directions = ['o', 'p', 'q', 'r', 'k', 'l', 'm', 'n']; 
  var sector = Math.round(direction / 45);
  return sector !== 8 ? directions[sector] : directions[0]; 
}

function addLeadingZero(num) {
  return num > 9 ? num : '0' + num;
}

function minutesOfADay(unixtime) {
  var date = new Date(unixtime);  
  return date.getHours() * 60 + date.getMinutes();
}

function timeFromUtc(utc) {
  var offset = new Date().getTimezoneOffset();  
  return new Date(utc * 1000  + offset * 60);
}

function getDateTimeStr(utc) {
  var time = timeFromUtc(utc);
  var date = addLeadingZero(time.getDate())+"."+addLeadingZero(time.getMonth() + 1);
  return date + " " + getTimeStr(utc);
}


function getTimeStr(utc) {  
  var time = timeFromUtc(utc);
  return addLeadingZero(time.getHours())+":"+addLeadingZero(time.getMinutes());
}

function locationForecast(pos) {
  isForecast = true;

  locationSuccess(pos);
}

function getRequestType() {
  return isForecast ? 'forecast' : 'weather';
}

function saveForecast(json) {
  var storageName = 'forecast';
}

function fillForecastData(data, fill_obj, index, json) {
    var keys = Object.keys(fill_obj);
    keys.forEach(function(key) {
      var val = fill_obj[key];
      var item_data = key.split('.').reduce(function (acc,item) {
          return acc[item];
        }, json);
      data[val + index] = processItemDispatcher(key, item_data, json.dt);

    });
    return data;
}

function get_forecast_matrix() {
    var forecastType = JSON.parse(localStorage.getItem('clay-settings')).ForecastType;
    switch (forecastType) {
      case 'ft_off':
        return null;
      case 'ft_3h': 
        return [0, 1, 2, 3];
      case 'ft_6h':
        return [0, 2, 4, 6];
      default: 
        return [0, 1, 2, 3];
    }
}

function processForecast(parsed) {
  var forecast_matrix = get_forecast_matrix();  
  var weather_data = {
     "WeatherMarkerForecast": true,
     "ForecastQty": forecast_matrix.length,
     "ForecastTime": parsed.list[0].dt
  };
  var fill_obj = {
    'main.temp': messageKeys['ForecastTemperature'],
    'weather.0.id': messageKeys['ForecastCondition'],
    'dt' : messageKeys['ForecastTimeStamp']
  };
  
  forecast_matrix.forEach(function(value, index) {
    fillForecastData(weather_data, fill_obj,
                    index, parsed.list[value]);

  });
  return weather_data;
}

function processItemDispatcher(item_key, item_data, dt) {
  switch (item_key) {
    case 'dt': 
      return processItemDateTime(item_data);
    case 'weather.0.id':
      return getCondition(item_data,undefined,undefined,dt);
    default: 
      return item_data;    
  }
}

function processItemDateTime(item) {
  return getDateTimeStr(item + 60);
  //59 min looks ugly, add one more minute
}

function saveSunData(sunrise, sunset) {
  var sunKey = "SunTimes";
  var sunItem = {
    "sunrise": sunrise * 1000,
    "sunset": sunset * 1000
  };
  localStorage.setItem(sunKey, JSON.stringify(sunItem));  
}

function parseResponse(json) {
  var weatherData = isForecast ? 
    processForecast(json) :    
    {
     "WeatherMarker": true,
     "WeatherTemperature": json.main.temp,
     "WeatherCondition": getCondition(json.weather[0].id, json.sys.sunrise * 1000, json.sys.sunset * 1000),
     "WeatherDesc": json.weather[0].description,
     "WeatherTimeStamp": json.dt,
     "WeatherPressure": Math.round(json.main.pressure * 0.75) - 14,
     "WeatherWindSpeed": json.wind.speed,
     "WeatherWindDirection": getWindDirection(json.wind.deg),
     "WeatherHumidity": json.main.humidity,
     "WeatherSunrise": getTimeStr(json.sys.sunrise),
     "WeatherSunset": getTimeStr(json.sys.sunset)
    };

    if (!isForecast) {      
      saveSunData(json.sys.sunrise, json.sys.sunset);
    }

    sendWeather(weatherData);
}

function locationSuccess(pos) {
  // We will request the weather here
  var weatherAPIKey = getWeatherAPIKey();
   if (weatherAPIKey == "not_set" || weatherAPIKey == 'invalid_api_key') {
     console.log("Weather ERROR: bad api key: " + weatherAPIKey);
     return ;
   }

  if (getRequestType() === 'forecast' && get_forecast_matrix() === null) {    
    return;
  }
  
  var url = 'http://api.openweathermap.org/data/2.5/'+getRequestType()+'?lat=' +
      pos.coords.latitude +
      '&lon=' + pos.coords.longitude +
      '&lang=' + i18n.getLang() +
      getTempUnits() + 
      '&appid=' + weatherAPIKey;// + 'ru';
    return xhrRequest(url, 'GET',
    function(responseText) {
    	var json = JSON.parse(responseText);
    	if (json.cod === 401) {
    		console.log("Waether ERROR: Invalid API key");
        setInvalidAPIKey();
        return;
    	}
      parseResponse(json);
    }
  );
}

function setInvalidAPIKey() {  
  message = 'invalid_api_key';
  clayHelperSetItem('WeatherAPIKey', message)

  var bad_api = {
      "ConfigMarker": true,
      "WeatherAPIKey": message
    };  
    sendWeather(bad_api);
}

function locationError(err) {  
  return {
  	"WeatherError":"Error requesting location!"
  }
}

//http://openweathermap.org/weather-conditions
function getCondition (owmCond, sunrise, sunset, time) {
  var isDay = isDayAt(sunrise, sunset, time);
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