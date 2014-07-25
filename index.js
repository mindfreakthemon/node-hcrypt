var assert = require('assert'),
	scarab = require('./build/Release/addon');

var crypto,
	operator;

crypto = new scarab.Crypto();
operator = new scarab.Operator(crypto.export());

function to_bin_array(x) {
	return x
		.toString(2)
		.split('')
		.map(Number)
		.reverse();
}

function from_bin_array(x) {
	return parseInt(x
		.reverse()
		.map(String)
		.join(''), 2);
}

function to_crypto_array(x) {
	return x.map(crypto.encrypt.bind(crypto));
}

function from_crypto_array(x) {
	return x.map(crypto.decrypt.bind(crypto));
}

function some_add(a, b) {
	/// PUBLIC DATA
//	console.log(a);
//	console.log(b);
	///

	var _c = null,
		l = a.length,
		r = new Array(l),
		i, v;

	for (i = 0; i < l; i++) {
		if (_c) {
			v = operator.fullAdd(_c, a[i], b[i]);
		} else {
			v = operator.halfAdd(a[i], b[i]);
		}

		r[i] = v[0];
		_c = v[1];
	}

	/// PUBLIC DATA
//	console.log(r);
	///

	return r;
}

function add(a, b) {
	/// INPUT
//	console.log(a, b);
	///

	var _a = to_bin_array(a),
		_b = to_bin_array(b),
		_al = _a.length,
		_bl = _b.length,
		_l, _t, _r;

	if (_al !== _bl) {
		_l = Math.abs(_al - _bl);
		_t = _al > _bl ? _b : _a;

		while (_l--) {
			_t.push(0);
		}
	}

	assert.equal(_a.length, _b.length);

	_a.push(0);
	_b.push(0);

	/// PRIVATE DATA
//	console.log(_a);
//	console.log(_b);

	_a = to_crypto_array(_a);
	_b = to_crypto_array(_b);

	_r = some_add(_a, _b);
	_r = from_crypto_array(_r);

	/// PRIVATE DATA
//	console.log(_r);

	_r = from_bin_array(_r);

	/// RESULT
//	console.log(_r);
	///

	return _r;
}

var x = 234234234,
	y = 4234234234,
	t = process.hrtime();

var r = add(x, y);

t = process.hrtime(t);

assert.equal(r, x + y);

console.log('took %d', t[0] + t[1] * 1e-9);
console.log('r = %d, should be %d', r, x + y);
