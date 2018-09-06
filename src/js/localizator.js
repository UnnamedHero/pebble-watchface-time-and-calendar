import getLocale from './locales';

const localizator = (config) => {
  const locale = getLocale();

  const getTranslatedValue = (val) => {
    const localizedstring = locale[val];
    return localizedstring || val;
  };

  return config.map((item) => {
    if (typeof item === 'string') {
      return getTranslatedValue(item);
    }

    const newItem = item;

    if (item.value) {
      newItem.value = getTranslatedValue(item.value);
    }
    if (item.label) {
      newItem.label = getTranslatedValue(item.label);
    }
    if (item.defaultValue) {
      newItem.defaultValue = getTranslatedValue(item.defaultValue);
    }
    if (item.items) {
      newItem.items = localizator(item.items);
    }
    if (item.options) {
      newItem.options = localizator(item.options);
    }

    return newItem;
  });
};

export default localizator;
