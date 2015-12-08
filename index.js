"use strict";

var assert = require('assert'),
	scarab = require('./lib/scarab');

var crypto = new scarab.Crypto(),
	publicKey = crypto.export(),
	operator = new scarab.Operator(publicKey);

var t = process.hrtime();

/* CLIENT HAS THE DATA */

const BITS_N = 15;
const TRY_IT = 20;

var a = 1,
	b = 4;

/* CLIENT ENCODES HIS DATA */

var _a = scarab.encrypt(crypto, a, BITS_N),
	_b = scarab.encrypt(crypto, b, BITS_N);

/* CLIENT SENDS publicKey and _a and _b TO THE SERVER */

/* SERVER PERFORMS THE OPERATION WITH THE _a and _b AND publicKey */

var _r = _a;


for (let i = 1; i <= TRY_IT; i++) {
	_r = scarab.add(operator, _r, _b);
	//console.log('add', scarab.decrypt(crypto, _r));

	scarab.recrypt(operator, _r);
	//console.log('rpt', scarab.decrypt(crypto, _r));
}

/* SERVER SENDS RESULT TO THE CLIENT */

/* CLIENT DECRYPTS THE RESULT */

var r = scarab.decrypt(crypto, _r);

t = process.hrtime(t);

//assert.equal(result, a + b);

console.log('took %d', t[0] + t[1] * 1e-9);
console.log('x =', scarab.to_bin_array(a + TRY_IT * b));
console.log('r =', scarab.to_bin_array(r));
console.log('r = %d, should be %d', r, a + TRY_IT * b);
