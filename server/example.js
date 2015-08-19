#!/usr/bin/env node

var DBus = require('dbus');

var dbus = new DBus();
var system = dbus.getBus('system');
var service = dbus.registerService('system', 'org.dbus.example');
var obj = service.createObject('/org/dbus/example');
var check = obj.createInterface('org.dbus.example');

check.addMethod('Method', { in: [ DBus.Define(String, 'uniqueName') ], out: DBus.Define(String, 'result') }, function(uniqueName, callback) {
	callback(uniqueName);
});

check.addSignal('Test', {
	types: [
		DBus.Define(String)
	]
});

setInterval(function() {
	check.emit('Test', 'Hello World!');
}, 3000);

check.update();
