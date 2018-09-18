const timeout = 5000;
const maxAttempts = 5;

const sender = (data, attempt) => {
  if (attempt > maxAttempts) {
    console.log('Message send FAILED');
    return;
  }

  const onSuccess = () => {
    console.log('Data sent to Pebble successfully!');
  };

  const onError = (nextAttempt, id, error) => {
    console.log(`Error sending data to Pebble! ${JSON.stringify(id)}: ${error}. Next Attempt: ${nextAttempt} in ${timeout} ms`);
    setTimeout(() => {
      sender(data, nextAttempt);
    }, timeout);
  };

  Pebble.sendAppMessage(data, onSuccess, onError.bind(undefined, attempt + 1)); //eslint-disable-line
};

export default data => sender(data, 1);
