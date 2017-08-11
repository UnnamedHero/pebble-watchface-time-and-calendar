
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

  function transaleValue(val) {
    var localizedstring = locale[val];
    return localizedstring ? localizedstring : val;
  }

  var newConfig = config.map(function(item) {
    if (item.value) {
      item.value = transaleValue(item.value);
    }
    if (item.label) {
      item.label = transaleValue(item.label);
    }
    if (item.defaultValue) {
      item.defaultValue = transaleValue(item.defaultValue);
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

