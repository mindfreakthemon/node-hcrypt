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

exports.to_bin_array = to_bin_array;
exports.from_bin_array = from_bin_array;

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

		if (i > 0 && i % 1 === 0) {
			_c = operator.recrypt(_c);
			//r[i] = operator.recrypt(r[i]);
		}
	}

	return r;
};

exports.recrypt = function (operator, a) {
	return a.map(operator.recrypt.bind(operator));
};

exports.encrypt = function (crypto, a, N) {
	var _a = to_bin_array(a);

	_a = align(_a, N);

	return to_crypto_array(crypto, _a);
};

exports.decrypt = function (crypto, r) {
	var _r = from_crypto_array(crypto, r);

	return from_bin_array(_r);
};