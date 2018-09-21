export default function (minified) {
  const clayConfig = this;

  const registerToggle = items => items.forEach((item) => {
    // const master = item[0];
    // const slave = item[1];
    // const [master, slave] = item;
    // Disabled due to babel polyfill error
    const masterItem = clayConfig.getItemByMessageKey(item[0]);
    if (!masterItem) {
      return;
    }
    const slaveItem = clayConfig.getItemByMessageKey(item[1]);
    masterItem.on('change', () => {
      if (masterItem.get()) {
        slaveItem.enable();
      } else {
        slaveItem.disable();
      }
    });
    masterItem.trigger('change');
  });


  const showGroups = groups => groups
    .forEach((group) => {
      clayConfig.getItemsByGroup(group)
        .forEach((item) => {
          item.show();
        });
    });

  const hideGroups = groups => groups
    .forEach((group) => {
      clayConfig.getItemsByGroup(group)
        .forEach((item) => {
          item.hide();
        });
    });

  const toggleSelectTable = [
    {
      selectId: 'WeatherProvider',
      values: {
        OWM: {
          hideGroups: [],
          showGroups: ['OWM', 'weather'],
          hook: () => {
            clayConfig.getItemById('WeatherLocationType').trigger('change');
          },
        },
        disable: {
          hideGroups: ['OWM', 'weather', 'weather_id'],
          showGroups: [],
        },
      },
    },
    {
      selectId: 'WeatherLocationType',
      values: {
        gps: {
          hideGroups: ['weather_id'],
          showGroups: [],
        },
        cid: {
          hideGroups: [],
          showGroups: ['weather_id'],
        },
      },
    },
  ];

  const registerSelectToggles = () => toggleSelectTable
    .forEach((select) => {
      const item = clayConfig.getItemById(select.selectId);
      if (!item) {
        return;
      }
      item.on('change', () => {
        const groups = select.values[item.get()];
        showGroups(groups.showGroups);
        hideGroups(groups.hideGroups);
        if (groups.hook) {
          groups.hook();
        }
      });
      item.trigger('change');
    });

  const compareVersions = (ver1, ver2) => {
    if (ver1 === ver2) {
      return 0;
    }
    const va1 = ver1.split('.');
    const va2 = ver2.split('.');
    const iter = (index) => {
      if (index > 2) {
        return 0;
      }
      const v1 = Number(va1[index]);
      const v2 = Number(va2[index]);
      if (v1 > v2) {
        return -1;
      }
      if (v1 < v2) {
        return 1;
      }
      return (index + 1);
    };
    return iter(0);
  };

  clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, () => {
    clayConfig.getItemById('versionId').set(`v. ${clayConfig.meta.userData.version}`);
    const updateElem = clayConfig.getItemById('updateId');
    clayConfig.getItemById('updateCheckBtn').on('click', () => {
      minified.$.request('get', 'https://raw.githubusercontent.com/UnnamedHero/pebble-watchface-time-and-calendar/master/package.json')
        .then((resp) => {
          const { version } = JSON.parse(resp);
          const versionCompareRes = compareVersions(version, clayConfig.meta.userData.version);
          if (versionCompareRes <= 0) {
            updateElem.set('You have actual version');
            return;
          }
          updateElem.set(`New version ${version} is available. Get it at <a href='https://drive.google.com/open?id=0B9g5sjcPqSJfRXpMUFE3Y2c1RGs'>Google Drive</a> `);
        })
        .error(() => {
          updateElem.set('Sorry, can\'t tell anything about new version availability');
        });
    });

    registerToggle([
      ['VibrateConnected', 'VibrateConnectedType'],
      ['VibrateDisconnected', 'VibrateDisconnectedType'],
      ['VibratePeriodic', 'VibratePeriodicType'],
      ['VibratePeriodic', 'VibratePeriodicPeroid'],
      ['QuietTime', 'QuietTimeBegin'],
      ['QuietTime', 'QuietTimeEnd'],
      ['ForecastType', 'SwitchBackTimeout'],
      ['PebbleShakeAction', 'SwitchBackTimeoutSeconds'],
      ['ColorTimeShift', 'ColorShiftTimeBegin'],
      ['ColorTimeShift', 'ColorShiftTimeEnd'],
      ['ColorTimeShift', 'ShiftFontColor'],
      ['ColorTimeShift', 'ShiftBackgroundColor'],
    ]);
    registerSelectToggles();
  });
}
