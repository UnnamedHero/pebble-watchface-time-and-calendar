export const messages = {
  weather_ok: 0,
  weather_disabled: 1,
  location_error: 2,
  api_key_invalid: 3,
  api_key_not_set: 4,
  invalid_location_id: 5,
  unknown_error: 6,
};

export const getWindDirectionSymbol = (direction) => {
  const directions = ['o', 'p', 'q', 'r', 'k', 'l', 'm', 'n'];
  const sector = Math.round(direction / 45);
  return sector !== 8 ? directions[sector] : directions[0];
};

export const conditions = {
  200: {
    name: 'thunderstorm with light rain',
    symbol: '`',
  },
  201: {
    name: 'thunderstorm with rain',
    symbol: '`',
  },
  202: {
    name: 'thunderstorm with heavy rain',
    symbol: '`',
  },
  210: {
    name: 'light thunderstorm',
    symbol: 'F',
  },
  211: {
    name: 'thunderstorm',
    symbol: 'F',
  },
  212: {
    name: 'heavy thunderstorm',
    symbol: 'F',
  },
  221: {
    name: 'ragged thunderstorm',
    symbol: 'F',
  },
  230: {
    name: 'thunderstorm with light drizzle',
    symbol: '_',
  },
  231: {
    name: 'thunderstorm with drizzle',
    symbol: '_',
  },
  232: {
    name: 'hunderstorm with heavy drizzle',
    symbol: '_',
  },
  300: {
    name: 'light intensity drizzle',
    symbol: '\'',
  },
  301: {
    name: 'drizzle',
    symbol: '\'',
  },
  302: {
    name: 'heavy intensity drizzle',
    symbol: '\'',
  },
  310: {
    name: 'light intensity drizzle rain',
    symbol: '\'',
  },
  311: {
    name: 'drizzle rain',
    symbol: '\'',
  },
  312: {
    name: 'heavy intensity drizzle rain',
    symbol: '\'',
  },
  313: {
    name: 'shower rain and drizzle',
    symbol: '\'',
  },
  314: {
    name: 'heavy shower rain and drizzle',
    symbol: '\'',
  },
  321: {
    name: 'shower drizzle',
    symbol: '\'',
  },
  500: {
    name: 'light rain',
    symbol: '6',
  },
  501: {
    name: 'moderate rain',
    symbol: '$',
  },
  502: {
    name: 'heavy intensity rain',
    symbol: 'a',
  },
  503: {
    name: 'very heavy rain',
    symbol: 'a',
  },
  504: {
    name: 'extreme rain',
    symbol: 'a',
  },
  511: {
    name: 'freezing rain',
    symbol: 's',
  },
  520: {
    name: 'light intensity shower rain',
    symbol: '*',
  },
  521: {
    name: 'shower rain',
    symbol: '*',
  },
  522: {
    name: 'heavy intensity shower rain',
    symbol: '*',
  },
  531: {
    name: 'ragged shower rain',
    symbol: '*',
  },
  600: {
    name: 'light snow',
    symbol: 't',
  },
  601: {
    name: 'snow',
    symbol: 't',
  },
  602: {
    name: 'heavy snow',
    symbol: 't',
  },
  611: {
    name: 'sleet',
    symbol: 'u',
  },
  612: {
    name: 'shower sleet',
    symbol: 'u',
  },
  615: {
    name: 'light rain and snow',
    symbol: 'u',
  },
  616: {
    name: 'rain and snow',
    symbol: 'u',
  },
  620: {
    name: 'light shower snow',
    symbol: 't',
  },
  621: {
    name: 'shower snow',
    symbol: 't',
  },
  622: {
    name: 'heavy shower snow',
    symbol: 't',
  },
  701: {
    name: 'mist',
    symbol: '?',
  },
  711: {
    name: 'smoke',
    symbol: 'А',
  },
  721: {
    name: 'haze',
    symbol: '<',
  },
  731: {
    name: 'sand, dust whirls',
    symbol: '<',
  },
  741: {
    name: 'fog',
    symbol: '?',
  },
  751: {
    name: 'sand',
    symbol: '<',
  },
  761: {
    name: 'dust',
    symbol: 'Б',
  },
  762: {
    name: 'volcanic ash',
    symbol: 'h',
  },
  771: {
    name: 'squalls',
    symbol: 'h',
  },
  781: {
    name: 'tornado',
    symbol: 'h',
  },
  800: {
    name: 'clear sky',
    symbol: ['I', 'N'],
  },
  801: {
    name: 'few clouds',
    symbol: ['b', '#'],
  },
  802: {
    name: 'scattered clouds',
    symbol: ['c', '#'],
  },
  803: {
    name: 'broken clouds',
    symbol: ['"', '#'],
  },
  804: {
    name: 'overcast clouds',
    symbol: '!',
  },
};
