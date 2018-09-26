import getLocation from './geoposition';

const locationStorageKey = 'geocache';

const saveLocation = location => localStorage.setItem(locationStorageKey, JSON.stringify(location));

const loadLocation = () => JSON.parse(localStorage.getItem(locationStorageKey)) || {};

export default async (options) => {
  try {
    const location = await getLocation(options);
    saveLocation(location);
    return location;
  } catch (e) {
    const location = loadLocation();
    // console.log('use cached location');
    if (Object.keys(location).length === 0) {
      throw new Error();
    }
    return location;
  }
};
