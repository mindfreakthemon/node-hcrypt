var scarab = require('./build/Release/addon');

var crypto,
	operator,
	i, v1, v2, e1, e2, r, l;

crypto = new scarab.Crypto();


console.log(JSON.stringify(crypto.export(), null, '\t'));
operator = new scarab.Operator(crypto.export());

for (i = 0, l = 10; i < l; i++) {
	v1 = Math.random() > 0.5 ? 1 : 0;
	v2 = Math.random() > 0.5 ? 1 : 0;
	e1 = crypto.encrypt(v1);
	e2 = crypto.encrypt(v2);

	r = operator.add(e1, e2);

	console.log('A: %d (%d), B: %d (%d), ER: %d, AR: %j',
		v1, crypto.decrypt(e1),
		v2, crypto.decrypt(e2),
		(v1 + v2) % 2, crypto.decrypt(r)
	);
}
