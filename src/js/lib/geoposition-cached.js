import getLocation from './geoposition';

const locationStorageKey = 'geocache';

const saveLocation = location => localStorage.setItem(locationStorageKey, JSON.stringify(location));

const loadLocation = () => JSON.parse(localStorage.getItem(locationStorageKey)) || {};

export default async (options) => {
  try {
    const location = await getLocation(options);
    const loc = {
      coords: {
        latitude: location.coords.latitude,
        longitude: location.coords.longitude,
      },
    };
    console.log(`save location ${JSON.stringify(loc)}`);
    saveLocation(loc);
    return loc;
  } catch (e) {
    const loc = loadLocation();
    console.log(`use cached location ${JSON.stringify(loc)}`);
    if (Object.keys(loc).length === 0) {
      console.log('oops, location cache is empty');
      throw new Error();
    }
    return loc;
  }
};
