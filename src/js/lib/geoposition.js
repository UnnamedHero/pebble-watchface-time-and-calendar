export default (options = { timeout: 10000, maximumAge: 1800000 }) => { // eslint-disable-line
  return new Promise((resolve, reject) => {
    navigator.geolocation.getCurrentPosition(resolve, reject, options);
  });
};
