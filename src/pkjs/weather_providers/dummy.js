var messageKeys = require('message_keys');
var sender = require('./../sender');
exports.getWeather = function (type) {

	var weather_data = type === 'forecast' ? 
		processForecast() : {	
	    "WeatherMarker": true,
	    "WeatherTemperature": 87,
	    "WeatherCondition" : 'Б',
		};
		sender.send(weather_data);

};

var jsonstr = '{"cod":"200","message":0.0082,"cnt":40,"list":[{"dt":1485799200,"main":{"temp":283.76,"temp_min":283.76,"temp_max":283.761,"pressure":1017.24,"sea_level":1026.83,"grnd_level":1017.24,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01n"}],"clouds":{"all":0},"wind":{"speed":7.27,"deg":15.0048},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-01-30 18:00:00"},{"dt":1485810000,"main":{"temp":282.56,"temp_min":282.56,"temp_max":282.563,"pressure":1020.06,"sea_level":1029.63,"grnd_level":1020.06,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01n"}],"clouds":{"all":0},"wind":{"speed":6.21,"deg":31.5035},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-01-30 21:00:00"},{"dt":1485820800,"main":{"temp":282.3,"temp_min":282.296,"temp_max":282.3,"pressure":1022.71,"sea_level":1032.27,"grnd_level":1022.71,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01d"}],"clouds":{"all":0},"wind":{"speed":6.71,"deg":51.0002},"rain":{},"sys":{"pod":"d"},"dt_txt":"2017-01-31 00:00:00"},{"dt":1485831600,"main":{"temp":282.27,"temp_min":282.265,"temp_max":282.27,"pressure":1023.68,"sea_level":1033.16,"grnd_level":1023.68,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01d"}],"clouds":{"all":0},"wind":{"speed":5.46,"deg":65.5},"rain":{},"sys":{"pod":"d"},"dt_txt":"2017-01-31 03:00:00"},{"dt":1485842400,"main":{"temp":282.656,"temp_min":282.656,"temp_max":282.656,"pressure":1023.75,"sea_level":1033.22,"grnd_level":1023.75,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01d"}],"clouds":{"all":0},"wind":{"speed":4.11,"deg":84.0055},"rain":{},"sys":{"pod":"d"},"dt_txt":"2017-01-31 06:00:00"},{"dt":1485853200,"main":{"temp":282.783,"temp_min":282.783,"temp_max":282.783,"pressure":1024.91,"sea_level":1034.46,"grnd_level":1024.91,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01n"}],"clouds":{"all":0},"wind":{"speed":3.6,"deg":89.0062},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-01-31 09:00:00"},{"dt":1485864000,"main":{"temp":283.335,"temp_min":283.335,"temp_max":283.335,"pressure":1025.21,"sea_level":1034.83,"grnd_level":1025.21,"humidity":100,"temp_kf":0},"weather":[{"id":803,"main":"Clouds","description":"broken clouds","icon":"04n"}],"clouds":{"all":68},"wind":{"speed":3.37,"deg":115},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-01-31 12:00:00"},{"dt":1485874800,"main":{"temp":284.657,"temp_min":284.657,"temp_max":284.657,"pressure":1023.65,"sea_level":1033.43,"grnd_level":1023.65,"humidity":100,"temp_kf":0},"weather":[{"id":802,"main":"Clouds","description":"scattered clouds","icon":"03n"}],"clouds":{"all":36},"wind":{"speed":3.32,"deg":177.501},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-01-31 15:00:00"},{"dt":1485885600,"main":{"temp":284.095,"temp_min":284.095,"temp_max":284.095,"pressure":1022.05,"sea_level":1031.79,"grnd_level":1022.05,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"02n"}],"clouds":{"all":8},"wind":{"speed":4.26,"deg":254.003},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-01-31 18:00:00"},{"dt":1485896400,"main":{"temp":283.211,"temp_min":283.211,"temp_max":283.211,"pressure":1021.33,"sea_level":1031.06,"grnd_level":1021.33,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01n"}],"clouds":{"all":0},"wind":{"speed":4.32,"deg":271.5},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-01-31 21:00:00"},{"dt":1485907200,"main":{"temp":284.179,"temp_min":284.179,"temp_max":284.179,"pressure":1020.57,"sea_level":1030.28,"grnd_level":1020.57,"humidity":100,"temp_kf":0},"weather":[{"id":801,"main":"Clouds","description":"few clouds","icon":"02d"}],"clouds":{"all":20},"wind":{"speed":10.16,"deg":256.504},"rain":{},"sys":{"pod":"d"},"dt_txt":"2017-02-01 00:00:00"},{"dt":1485918000,"main":{"temp":284.829,"temp_min":284.829,"temp_max":284.829,"pressure":1017.39,"sea_level":1027.23,"grnd_level":1017.39,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"02d"}],"clouds":{"all":8},"wind":{"speed":13.76,"deg":259.502},"rain":{},"sys":{"pod":"d"},"dt_txt":"2017-02-01 03:00:00"},{"dt":1485928800,"main":{"temp":285.702,"temp_min":285.702,"temp_max":285.702,"pressure":1014.64,"sea_level":1024.23,"grnd_level":1014.64,"humidity":100,"temp_kf":0},"weather":[{"id":801,"main":"Clouds","description":"few clouds","icon":"02d"}],"clouds":{"all":12},"wind":{"speed":12.75,"deg":264.001},"rain":{},"sys":{"pod":"d"},"dt_txt":"2017-02-01 06:00:00"},{"dt":1485939600,"main":{"temp":285.494,"temp_min":285.494,"temp_max":285.494,"pressure":1014.1,"sea_level":1023.71,"grnd_level":1014.1,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01n"}],"clouds":{"all":0},"wind":{"speed":12.33,"deg":271.5},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-02-01 09:00:00"},{"dt":1485950400,"main":{"temp":285.16,"temp_min":285.16,"temp_max":285.16,"pressure":1014.54,"sea_level":1024.27,"grnd_level":1014.54,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01n"}],"clouds":{"all":0},"wind":{"speed":12.21,"deg":278.504},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-02-01 12:00:00"},{"dt":1485961200,"main":{"temp":284.161,"temp_min":284.161,"temp_max":284.161,"pressure":1016.13,"sea_level":1025.81,"grnd_level":1016.13,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01n"}],"clouds":{"all":0},"wind":{"speed":12.21,"deg":281},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-02-01 15:00:00"},{"dt":1485972000,"main":{"temp":282.63,"temp_min":282.63,"temp_max":282.63,"pressure":1017.33,"sea_level":1027,"grnd_level":1017.33,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01n"}],"clouds":{"all":0},"wind":{"speed":9.3,"deg":297.001},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-02-01 18:00:00"},{"dt":1485982800,"main":{"temp":281.856,"temp_min":281.856,"temp_max":281.856,"pressure":1018.51,"sea_level":1028.24,"grnd_level":1018.51,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01n"}],"clouds":{"all":0},"wind":{"speed":8.91,"deg":288},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-02-01 21:00:00"},{"dt":1485993600,"main":{"temp":281.978,"temp_min":281.978,"temp_max":281.978,"pressure":1019.22,"sea_level":1028.89,"grnd_level":1019.22,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01d"}],"clouds":{"all":0},"wind":{"speed":9.15,"deg":286.504},"rain":{},"sys":{"pod":"d"},"dt_txt":"2017-02-02 00:00:00"},{"dt":1486004400,"main":{"temp":283.706,"temp_min":283.706,"temp_max":283.706,"pressure":1017.36,"sea_level":1026.99,"grnd_level":1017.36,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01d"}],"clouds":{"all":0},"wind":{"speed":8.95,"deg":288.003},"rain":{},"sys":{"pod":"d"},"dt_txt":"2017-02-02 03:00:00"},{"dt":1486015200,"main":{"temp":285.125,"temp_min":285.125,"temp_max":285.125,"pressure":1016.41,"sea_level":1026.05,"grnd_level":1016.41,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01d"}],"clouds":{"all":0},"wind":{"speed":7.56,"deg":294.002},"rain":{},"sys":{"pod":"d"},"dt_txt":"2017-02-02 06:00:00"},{"dt":1486026000,"main":{"temp":284.755,"temp_min":284.755,"temp_max":284.755,"pressure":1018.18,"sea_level":1027.75,"grnd_level":1018.18,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01n"}],"clouds":{"all":0},"wind":{"speed":8.31,"deg":290.504},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-02-02 09:00:00"},{"dt":1486036800,"main":{"temp":283.765,"temp_min":283.765,"temp_max":283.765,"pressure":1019.04,"sea_level":1028.51,"grnd_level":1019.04,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01n"}],"clouds":{"all":0},"wind":{"speed":8.87,"deg":283.001},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-02-02 12:00:00"},{"dt":1486047600,"main":{"temp":283.809,"temp_min":283.809,"temp_max":283.809,"pressure":1018.6,"sea_level":1028.26,"grnd_level":1018.6,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01n"}],"clouds":{"all":0},"wind":{"speed":8.73,"deg":281.001},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-02-02 15:00:00"},{"dt":1486058400,"main":{"temp":283.788,"temp_min":283.788,"temp_max":283.788,"pressure":1018.68,"sea_level":1028.33,"grnd_level":1018.68,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01n"}],"clouds":{"all":0},"wind":{"speed":7.8,"deg":281.503},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-02-02 18:00:00"},{"dt":1486069200,"main":{"temp":283.569,"temp_min":283.569,"temp_max":283.569,"pressure":1019.28,"sea_level":1028.92,"grnd_level":1019.28,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01n"}],"clouds":{"all":0},"wind":{"speed":5.92,"deg":284.002},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-02-02 21:00:00"},{"dt":1486080000,"main":{"temp":284.11,"temp_min":284.11,"temp_max":284.11,"pressure":1020.75,"sea_level":1030.35,"grnd_level":1020.75,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01d"}],"clouds":{"all":0},"wind":{"speed":1.83,"deg":284.506},"rain":{},"sys":{"pod":"d"},"dt_txt":"2017-02-03 00:00:00"},{"dt":1486090800,"main":{"temp":285.034,"temp_min":285.034,"temp_max":285.034,"pressure":1020.08,"sea_level":1029.7,"grnd_level":1020.08,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01d"}],"clouds":{"all":0},"wind":{"speed":1.01,"deg":172.002},"rain":{},"sys":{"pod":"d"},"dt_txt":"2017-02-03 03:00:00"},{"dt":1486101600,"main":{"temp":286.158,"temp_min":286.158,"temp_max":286.158,"pressure":1018.77,"sea_level":1028.39,"grnd_level":1018.77,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01d"}],"clouds":{"all":0},"wind":{"speed":0.71,"deg":181.009},"rain":{},"sys":{"pod":"d"},"dt_txt":"2017-02-03 06:00:00"},{"dt":1486112400,"main":{"temp":286.138,"temp_min":286.138,"temp_max":286.138,"pressure":1019.17,"sea_level":1028.73,"grnd_level":1019.17,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01n"}],"clouds":{"all":0},"wind":{"speed":1.51,"deg":85.0026},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-02-03 09:00:00"},{"dt":1486123200,"main":{"temp":285.952,"temp_min":285.952,"temp_max":285.952,"pressure":1019.74,"sea_level":1029.36,"grnd_level":1019.74,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01n"}],"clouds":{"all":0},"wind":{"speed":3.68,"deg":76.5032},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-02-03 12:00:00"},{"dt":1486134000,"main":{"temp":285.828,"temp_min":285.828,"temp_max":285.828,"pressure":1019.23,"sea_level":1029.02,"grnd_level":1019.23,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01n"}],"clouds":{"all":0},"wind":{"speed":3.36,"deg":98.0014},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-02-03 15:00:00"},{"dt":1486144800,"main":{"temp":285.168,"temp_min":285.168,"temp_max":285.168,"pressure":1018.15,"sea_level":1027.96,"grnd_level":1018.15,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01n"}],"clouds":{"all":0},"wind":{"speed":1.66,"deg":333.002},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-02-03 18:00:00"},{"dt":1486155600,"main":{"temp":283.818,"temp_min":283.818,"temp_max":283.818,"pressure":1018.65,"sea_level":1028.36,"grnd_level":1018.65,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01n"}],"clouds":{"all":0},"wind":{"speed":3.77,"deg":359.001},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-02-03 21:00:00"},{"dt":1486166400,"main":{"temp":283.157,"temp_min":283.157,"temp_max":283.157,"pressure":1019.84,"sea_level":1029.41,"grnd_level":1019.84,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01d"}],"clouds":{"all":0},"wind":{"speed":2.91,"deg":18.5013},"rain":{},"sys":{"pod":"d"},"dt_txt":"2017-02-04 00:00:00"},{"dt":1486177200,"main":{"temp":283.494,"temp_min":283.494,"temp_max":283.494,"pressure":1018.56,"sea_level":1028.13,"grnd_level":1018.56,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01d"}],"clouds":{"all":0},"wind":{"speed":0.22,"deg":280.501},"rain":{},"sys":{"pod":"d"},"dt_txt":"2017-02-04 03:00:00"},{"dt":1486188000,"main":{"temp":285.263,"temp_min":285.263,"temp_max":285.263,"pressure":1016.77,"sea_level":1026.41,"grnd_level":1016.77,"humidity":100,"temp_kf":0},"weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01d"}],"clouds":{"all":0},"wind":{"speed":1.88,"deg":273.501},"rain":{},"sys":{"pod":"d"},"dt_txt":"2017-02-04 06:00:00"},{"dt":1486198800,"main":{"temp":285.927,"temp_min":285.927,"temp_max":285.927,"pressure":1017.03,"sea_level":1026.52,"grnd_level":1017.03,"humidity":100,"temp_kf":0},"weather":[{"id":801,"main":"Clouds","description":"few clouds","icon":"02n"}],"clouds":{"all":12},"wind":{"speed":3.02,"deg":269.505},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-02-04 09:00:00"},{"dt":1486209600,"main":{"temp":285.906,"temp_min":285.906,"temp_max":285.906,"pressure":1016.93,"sea_level":1026.64,"grnd_level":1016.93,"humidity":100,"temp_kf":0},"weather":[{"id":802,"main":"Clouds","description":"scattered clouds","icon":"03n"}],"clouds":{"all":44},"wind":{"speed":3.77,"deg":261.501},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-02-04 12:00:00"},{"dt":1486220400,"main":{"temp":285.51,"temp_min":285.51,"temp_max":285.51,"pressure":1016.46,"sea_level":1026.16,"grnd_level":1016.46,"humidity":100,"temp_kf":0},"weather":[{"id":802,"main":"Clouds","description":"scattered clouds","icon":"03n"}],"clouds":{"all":44},"wind":{"speed":2.97,"deg":283.001},"rain":{},"sys":{"pod":"n"},"dt_txt":"2017-02-04 15:00:00"}],"city":{"id":1907296,"name":"Tawarano","coord":{"lat":35.0164,"lon":139.0077},"country":"none"}}';

function fillForecastData(data, fill_obj, index, json) {
    var keys = Object.keys(fill_obj);
    keys.forEach(function(key) {
      var val = fill_obj[key];
      var item_data = key.split('.').reduce(function (acc,item) {
          return acc[item];
        }, json);
      data[val + index] = processItemDispatcher(key, item_data);
      console.log(data[val+index]);
    });
    return data;
}

function processForecast() {
  console.log("Process forecast");
  var forecast_matrix = [0, 1, 2, 3];
  var parsed = JSON.parse(jsonstr);
  var weather_data = {
     "WeatherMarkerForecast": true,
     "ForecastQty": forecast_matrix.length,
      "ForecastTime": parsed.list[0].dt
  };

  //var forecast_key = messageKeys['ForecastTemperature'];
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

function processItemDispatcher(item_key, item_data) {
  switch (item_key) {
    case 'dt': 
      return processItemDateTime(item_data);
    default: 
      return item_data;    
  }
}

function processItemDateTime(item) {
  return getDateTimeStr(item + 60);
}

function addLeadingZero(num) {
  return num > 9 ? num : '0' + num;
}

function timeFromUtc (utc) {
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