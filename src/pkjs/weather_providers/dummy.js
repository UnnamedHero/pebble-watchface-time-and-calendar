exports.getWeather = function () {

	var weather_data = {
	    "WeatherMarker": true,
	    "WeatherTemperature": 87,
	    "WeatherCondition" : 'Б',
		};
    Pebble.sendAppMessage(weather_data, function(e) {
      console.log('Weather info sent to Pebble successfully!');
    }, function(e) {
      console.log('Error sending weather info to Pebble!');
    });  
};





