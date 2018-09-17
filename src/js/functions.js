export default function () {
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

  clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, () => {
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
