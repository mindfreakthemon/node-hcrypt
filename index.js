var scarab = require('./build/Release/addon');

var obj = new scarab.Scarab(),
	i, v, e, d, l;



for (i = 0, l = 10; i < l; i++) {
	v = Math.random() > 0.5 ? 1 : 0;
	e = obj.encrypt(v);
	d = obj.decrypt(e);

	console.log('VALUE: %d, RESULT: %d', v, d);
}
