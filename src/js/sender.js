var timeout = 5000;
var maxAttempts = 5;

function sender(data, attempt) {
	if (attempt > maxAttempts) {
		console.log('Message send FAILED');
		return;
	}
    var cb = function (currAttempt, data, res, error) {        
        var nextAttempt = currAttempt + 1;
        console.log('Error sending data to Pebble! Next Attempt:'+ nextAttempt+' in '+timeout+'ms');
        setTimeout(function() {     
            sender(data, nextAttempt);
        }, timeout);        
    };
    var thisCb = cb.bind(undefined, attempt, data);
	Pebble.sendAppMessage(data, function(e) {
     	console.log('Data sent to Pebble successfully!');
     }, thisCb);  
}

exports.send = function(data) {
	sender(data, 1);
};