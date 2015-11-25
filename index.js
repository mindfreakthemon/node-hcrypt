var assert = require('assert'),
	scarab = require('./lib/scarab');

var crypto = new scarab.Crypto(),
	operator = new scarab.Operator(crypto.export());

var t = process.hrtime();

/* CLIENT HAS THE DATA */

var a = 22222,
	b = 321;

/* CLIENT ENCODES HIS DATA */

var _encoded = scarab.encrypt(crypto, a, b);

/* CLIENT SENDS crypto.export() and _encoded TO THE SERVER */

console.log(_encoded);

/* SERVER PERFORMS THE OPERATION WITH THE _encoded AND crypto.export() */

var _encoded_result = scarab.add(operator, _encoded.a, _encoded.b);

/* SERVER SENDS RESULT TO THE CLIENT */

/* CLIENT DECRYPTS THE RESULT */

var result = scarab.decrypt(crypto, _encoded_result);

t = process.hrtime(t);

assert.equal(result, a + b);

console.log('took %d', t[0] + t[1] * 1e-9);
console.log('r = %d, should be %d', result, a + b);
