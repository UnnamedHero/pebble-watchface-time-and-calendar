
function getLang () {
  const countryCodes = {
    'ru-RU' : 'ru',
    'en-US' : 'en'
  };
  const lang = countryCodes[navigator.language];
  return lang ? lang : 'en';
}

exports.getLang = getLang;

  var currentLocaleModulePath = './locales/' + getLang();
  var defaultLocaleModulePath = './locales/en';

  try {
    var locale = require('./locales/'+getLang());
  } catch (ex) {
    var locale = require('./locales/en');
  }

function localizator(config) {

  function translateValue(val) {
    var localizedstring = locale[val];
    return localizedstring ? localizedstring : val;
  }

  var newConfig = config.map(function(item) {

    if (typeof item === 'string') {
      item = translateValue(item);
    }

    if (item.value) {
      item.value = translateValue(item.value);
    }

    if (item.label) {
      item.label = translateValue(item.label);
    }

    if (item.defaultValue) {
      item.defaultValue = translateValue(item.defaultValue);
    }

    if (item.items) {
      item.items = localizator(item.items);
    }

    if (item.options) {
      item.options = localizator(item.options);
    }
    
    return item;
  });
  return newConfig;
};



exports.translate = localizator;

