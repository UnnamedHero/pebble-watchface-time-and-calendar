module.exports = function(minified) {
  var clayConfig = this;

  function toggleItem(master, slave) {
       master.get() ? slave.enable() : slave.disable();
  }

  function registerToggle(master, slave) {
    var masterItem = clayConfig.getItemByMessageKey(master);
    var slaveItem = clayConfig.getItemByMessageKey(slave);
    masterItem.on('change', function() {
      if (master === 'ForecastType') {
        this.get() !== 'ft_off' ? slaveItem.enable() : slaveItem.disable();
      } else {
        this.get() ? slaveItem.enable() : slaveItem.disable();
      }
    });
    masterItem.trigger('change');
  }

  clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function() {
    registerToggle('VibrateConnected', 'VibrateConnectedType');
    registerToggle('VibrateDisconnected', 'VibrateDisconnectedType');
    registerToggle('VibratePeriodic', 'VibratePeriodicType');
    registerToggle('VibratePeriodic', 'VibratePeriodicPeroid');
    registerToggle('QuietTime', 'QuietTimeBegin');
    registerToggle('QuietTime', 'QuietTimeEnd');
    registerToggle('ForecastType', 'SwitchBackTimeout');
    //registerToggle('ForecastType', 'ForecastDummy');    
  });
};
