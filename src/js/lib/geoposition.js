export default (options = {
  timeout: 10000,
  maximumAge: 1800000,
}) => new Promise((resolve, reject) => {
  navigator.geolocation.getCurrentPosition(resolve, reject, options);
});
