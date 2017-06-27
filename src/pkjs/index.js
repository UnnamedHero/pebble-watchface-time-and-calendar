// Import the Clay package
var Clay = require('pebble-clay');
// Load our Clay configuration file
var clayConfig = require('./config');
//var customFunction = require('./custom-function');
// Initialize Clay
var clay = new Clay(clayConfig);
// var clay = new Clay(clayConfig, customFunctions, {
//   locales: {
//     en_US: require('./locale/config/en-us.json'),
//     ru_RU: require('./locale/config/ru-ru.json'),
//   },
//   bundleAllLocales: true
// });
