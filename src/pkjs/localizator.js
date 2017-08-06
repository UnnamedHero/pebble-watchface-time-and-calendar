module.exports = function () {

function getLang() {
  var countryCodes = {
    'ru-RU' : 'ru',
    'en-US' : 'en'
  }
  var lang = countryCodes[navigator.language];
  return lang ? lang : 'en';
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
}


};