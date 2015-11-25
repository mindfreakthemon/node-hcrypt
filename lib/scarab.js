var assert = require('assert'),
	scarab = require('../build/Release/addon');

exports.Crypto = scarab.Crypto;
exports.Operator = scarab.Operator;

function align(a, b) {
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

	return {
		a: _a,
		b: _b
	};
}


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

function to_crypto_array(crypto, x) {
	return x.map(crypto.encrypt.bind(crypto));
}

function from_crypto_array(crypto, x) {
	return x.map(crypto.decrypt.bind(crypto));
}

exports.add = function (operator, a, b) {
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

	return r;
};

exports.encrypt = function (crypto, a, b) {
	var _input = align(a, b);

	return {
		a: to_crypto_array(crypto, _input.a),
		b: to_crypto_array(crypto, _input.b)
	};
};

exports.decrypt = function (crypto, r) {
	var _r = from_crypto_array(crypto, r);

	return from_bin_array(_r);
};