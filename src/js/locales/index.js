import ru from 'ru';

const locales = {
  en: {},
  ru
};

export default () => {
  const lang = countryCodes[navigator.language] || 'en';
  return locales[lang];
}