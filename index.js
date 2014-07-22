var scarab = require('./build/Release/addon');

var obj = new scarab.Scarab();
console.log( obj.encrypt(1) );
