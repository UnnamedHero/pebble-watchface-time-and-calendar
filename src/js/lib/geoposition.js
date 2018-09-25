export default (options = {
  timeout: 10000,
  maximumAge: 3600000,
}) => new Promise((resolve, reject) => {
  navigator.geolocation.getCurrentPosition(resolve, reject, options);
});
