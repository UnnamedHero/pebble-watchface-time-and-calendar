var timeout = 5000;
var maxAttempts = 5;

function sender(data, attempt) {
	var current = typeof attempt !== undefined ? attempt : 1;
	if (current > maxAttempts) {
		console.log('Message send FAILED');
		return;
	}

	Pebble.sendAppMessage(data, function(e) {
    	console.log('Data sent to Pebble successfully!');
    }, function(e) {
    	var nextAttempt = current + 1;
    	console.log('Error sending data to Pebble! Next Attempt:'+ nextAttempt+' in '+timeout+'ms');
    	setTimeout(function() {    	
    		sender(data, nextAttempt);
    	}, timeout);
    });  
}

exports.send = function(data) {
	sender(data);
};