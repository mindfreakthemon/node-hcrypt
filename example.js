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

let _rM, rM, _rA, rA;

let a = 2,
	b = 8;

console.log('initial data: a = %d, b = %d', a, b);

/* CLIENT ENCODES HIS DATA */

let _a = scarab.encrypt(crypto, a, BITS_N),
	_b = scarab.encrypt(crypto, b, BITS_N);

/* CLIENT SENDS publicKey and _a and _b TO THE SERVER */

/* SERVER PERFORMS THE OPERATION WITH THE _a and _b AND publicKey */

_rM = scarab.mul(operator, _a, _b);
_rA = _a;

for (let i = 1; i <= TRY_IT; i++) {
	_rA = scarab.add(operator, _rA, _b);

	scarab.recrypt(operator, _rA);
}

/* SERVER SENDS RESULT TO THE CLIENT */

/* CLIENT DECRYPTS THE RESULT */

rM = scarab.decrypt(crypto, _rM);
rA = scarab.decrypt(crypto, _rA);

console.log('rM = %d, should be %d', rM, a * b);
console.log('rA = %d, should be %d', rA, a + TRY_IT * b);
assert.equal(rM, a + TRY_IT * b);
assert.equal(rA, a * b);

t = process.hrtime(t);
console.log('took %d', t[0] + t[1] * 1e-9);
