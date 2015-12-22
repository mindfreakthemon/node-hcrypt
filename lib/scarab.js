var assert = require('assert'),
	scarab = require('../build/Release/addon');

exports.Crypto = scarab.Crypto;
exports.Operator = scarab.Operator;

function align(_a, N) {
	var _al = _a.length,
		_l;

	if (_al < N) {
		_l = N - _al;

		while (_l--) {
			_a.push(0);
		}
	}

	return _a;
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

/* Crypto extensions */

exports.to_bin_array = to_bin_array;
exports.from_bin_array = from_bin_array;

exports.encrypt = function (crypto, a, N) {
	var _a = to_bin_array(a);

	_a = align(_a, N);

	return to_crypto_array(crypto, _a);
};

exports.decrypt = function (crypto, r) {
	var _r = from_crypto_array(crypto, r);

	return from_bin_array(_r);
};


/* Operator extensions */

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

		// recrypt
		_c = operator.recrypt(_c);
	}

	return r;
};

exports.mul = function (operator, a, b) {
	var _c = null,
		r = null,
		l = a.length,
		_a = a.slice(0),
		_t = new Array(l),
		i, j;

	for (i = 0; i < l; i++) {
		_c = b[i];

		for (j = 0; j < l; j++) {
			_t[j] = operator.mul(_c, _a[j]);
			// recrypt
			_t[j] = operator.recrypt(_t[j]);
		}

		if (r) {
			r = exports.add(operator, r, _t);
		} else {
			r = _t;
		}

		// recrypt
		r = exports.recrypt(operator, r);

		_a = exports.add(operator, _a, _a);
		// recrypt
		_a = exports.recrypt(operator, _a);
	}

	return r;
};

exports.recrypt = function (operator, a) {
	return a.map(operator.recrypt.bind(operator));
};
