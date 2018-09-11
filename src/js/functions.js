export default function () {
  const clayConfig = this;

  // const toggleItem = (master, slave) => {
  //      master.get() ? slave.enable() : slave.disable();
  // }

  // const dataToggle = {
  //   ForecastType: 'ft_off',
  // };

  // const dataToggleReversed = {
  //   PebbleShakeAction: '2',
  // };

  const registerToggle = items => items.forEach((item) => {
    // const master = item[0];
    // const slave = item[1];
    // // const [master, slave] = item;
    // Disabled due to babel polyfill error
    const masterItem = clayConfig.getItemByMessageKey(item[0]);
    console.log(`i: ${item[0]}, ${masterItem}`);
    const slaveItem = clayConfig.getItemByMessageKey(item[1]);
    masterItem.on('change', () => {
      if (masterItem.get()) {
        slaveItem.enable();
      } else {
        slaveItem.disable();
      }
    });
    masterItem.trigger('change');
    //   if (dataToggle[master]) {
    //     this.get() !== dataToggle[master] ? slaveItem.enable() : slaveItem.disable();
    //   } else if (dataToggleReversed[master]) {
    //       this.get() !== dataToggleReversed[master] ? slaveItem.disable() : slaveItem.enable();
    //   } else {
    //     this.get() ? slaveItem.enable() : slaveItem.disable();
    //   }
    // });
  });

  // function registerGroupHideById(master, masterValue, group) {
  //   var masterItem = clayConfig.getItemById(master);

  //   var items = clayConfig.getItemsByGroup(group);
  //   masterItem.on('change', function () {
  //     items.forEach(function(item) {
  //       masterItem.get() === masterValue ? item.show() : item.hide();
  //     });
  //   });
  //   masterItem.trigger('change');
  // }

  // function toggleItemByGroup(showGroup, hideGroup) {
  //   clayConfig.getItemsByGroup(showGroup).forEach(function(item) {
  //     item.show();
  //   });
  //   clayConfig.getItemsByGroup(hideGroup).forEach(function(item) {
  //     item.hide();
  //   });
  // }

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


  const weatherF = () => {
    switch (this.get()) {
      case 'OWM':
        showGroups(['OWM', 'weather']);
        clayConfig.getItemById('WeatherLocationType').trigger('change');
        break;
      case 'disable':
        hideGroups(['OWM', 'weather', 'weather_id']);
        break;
      default:
        break;
    }
  };

  const weatherToggle = () => {
    const masterItem = clayConfig.getItemByMessageKey('WeatherProvider');
    masterItem.on('change', weatherF);
    weatherF.call(masterItem);
  };

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
      ['WeatherLocationType', 'cid', 'weather_id'],
    ]);
    weatherToggle();
  });
}
