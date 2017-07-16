module.exports = function(minified) {
  var clayConfig = this;

  //  function togglePebbleBatteryBarPercents() {
  //    this.get() ? clayConfig.getItemById('PebbleBatteryBarPercents').enable() :
  //      clayConfig.getItemById('PebbleBatteryBarPercents').disable();
  // }
  //
  function toggleItem(master, slave) {
    //var item = clayConfig.getItemById(id);
       master.get() ? slave.enable() : slave.disable();
  }

  function registerToggle(master, slave) {
    var masterItem = clayConfig.getItemByMessageKey(master);
    var slaveItem = clayConfig.getItemByMessageKey(slave);
    masterItem.on('change', function() {
      this.get() ? slaveItem.enable() : slaveItem.disable();
    });
    masterItem.trigger('change');
  }

  // clayConfig.on(clayConfig.EVENTS.BEFORE_BUILD, function () {
  //   console.log("before build");
    //var items = clayConfig.getAllItems();
    //console.log ("Len:"+items.length);
     //items.forEach(fucntion(item) {
    //   console.log(item.keys());
     //});
  // });

  clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function() {

    registerToggle ('VibrateConnected', 'VibrateConnectedType');
    registerToggle ('VibrateDisconnected', 'VibrateDisconnectedType');
    registerToggle ('VibratePeriodic', 'VibratePeriodicType');
    registerToggle ('VibratePeriodic', 'VibratePeriodicPeroid');

    clayConfig.getItemById('ConfigMarker').hide();
  });
};
