import ru from './ru';

const countryCodes = {
  'ru-RU': 'ru',
  'en-US': 'en',
};

const locales = {
  en: {},
  ru,
};

export default () => {
  const lang = countryCodes[navigator.language] || 'en';
  return locales[lang];
};
