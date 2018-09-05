module.exports = function(minified) {
  var clayConfig = this;

  function toggleItem(master, slave) {
       master.get() ? slave.enable() : slave.disable();
  }

  var dataToggle = {
    'ForecastType': 'ft_off',
    
  };

  var dataToggleReversed = {
    'PebbleShakeAction': '2',
  };

  function registerToggle(master, slave, type) {    
    var masterItem = clayConfig.getItemByMessageKey(master);
    var slaveItem = clayConfig.getItemByMessageKey(slave);
    masterItem.on('change', function() {
      if (dataToggle[master]) {
        this.get() !== dataToggle[master] ? slaveItem.enable() : slaveItem.disable();
      } else if (dataToggleReversed[master]) {
          this.get() !== dataToggleReversed[master] ? slaveItem.disable() : slaveItem.enable();
      } else {
        this.get() ? slaveItem.enable() : slaveItem.disable();
      }
    });
    masterItem.trigger('change');
  }

  function registerGroupHideById(master, masterValue, group) {
    var masterItem = clayConfig.getItemById(master);
    
    var items = clayConfig.getItemsByGroup(group);
    masterItem.on('change', function () {
      items.forEach(function(item) {
        masterItem.get() === masterValue ? item.show() : item.hide();
      });  
    });
    masterItem.trigger('change');    
  }

  function toggleItemByGroup(showGroup, hideGroup) {
    clayConfig.getItemsByGroup(showGroup).forEach(function(item) {
      item.show();
    });
    clayConfig.getItemsByGroup(hideGroup).forEach(function(item) {
      item.hide();
    });
  }

  function weatherF() {
    switch (this.get()) {
      case 'OWM': 
        toggleItemByGroup('OWM', '');
        toggleItemByGroup('weather', '');
        clayConfig.getItemById('WeatherLocationType').trigger('change');
        break;
      case 'disable':
        toggleItemByGroup('', 'weather');
        toggleItemByGroup('', 'OWM');
        toggleItemByGroup('', 'weather_id');
        break;
    }
  }

  function weatherToggle() {
    var masterItem = clayConfig.getItemByMessageKey('WeatherProvider');
    masterItem.on('change', weatherF);
    weatherF.call(masterItem);
  }

  clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function() {
    registerToggle('VibrateConnected', 'VibrateConnectedType');
    registerToggle('VibrateDisconnected', 'VibrateDisconnectedType');
    registerToggle('VibratePeriodic', 'VibratePeriodicType');
    registerToggle('VibratePeriodic', 'VibratePeriodicPeroid');
    registerToggle('QuietTime', 'QuietTimeBegin');
    registerToggle('QuietTime', 'QuietTimeEnd');
    registerToggle('ForecastType', 'SwitchBackTimeout');
    registerToggle('PebbleShakeAction', 'SwitchBackTimeoutSeconds');
    registerGroupHideById('WeatherLocationType', 'cid', 'weather_id');   
    weatherToggle();
  });
};
