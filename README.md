node-hcrypt
===========
### Fully homomorphic encryption implementation for node.js

[![CircleCI](https://circleci.com/gh/mindfreakthemon/node-hcrypt.svg?style=svg)](https://circleci.com/gh/mindfreakthemon/node-hcrypt)

This package provides an API for node.js application to use homomorphic encryption to perform computations.

Only integers are currently supported.

Operations supported: algebraic addition and multiplication.

----
#### To Install

```npm install node-hcrypt```

----
Dependencies
======
* GMP: The GNU Multiple Precision Arithmetic Library — https://gmplib.org/ (or MPIR in GMP mode; required by FLINT)
* MPIR: The GNU Multiple Precision Integers and Rationals — http://mpir.org/ (or GMP; required by FLINT)
* MPFR: The GNU Multiple Precision Floating-Point Reliably — http://www.mpfr.org/ (required by FLINT)
* FLINT: Fast Library for Number Theory — http://flintlib.org/ (version 2.4.1+)

Debian-based
------
```apt-get -yy install libflint-2.5.2 libflint-dev libgmp-dev libmpfr-dev gcc g++ make automake autoconf gyp```.

RHEL-based
------
```yum install gcc gcc-c++ make automake autoconf mpfr-devel gmp-devel gyp```.

Note:
  * You will need `epel-release` to download `gyp`
  * Flint must be compiled from sources

Windows
-------

You can compile library under cygwin-x64 on Windows too. Just make sure you have installed all the following libs:
* gcc-core
* gcc-g++
* gmp
* mpfr
* autoconf2.1
* automake1.14
* libmprf4
* libmprf-devel
* libgmp10
* libgmpxx4
* libgmp-devel
* make

Usage
=======
You can use the library as demonstrated in example.js.

----

#### API

The module exports two main object types: Crypto and Operator. 
Crypto provides with encrypting / decrypting functionality while Operator deals with the operations on encrypted data.
Module also exports helpers for converting any arbitrary numbers and performing operations with them, but they will be integrated into the mentioned above object types soon. 

##### Crypto
```
var hcrypt = require('node-hcrypt');

var crypto = new hcrypt.Crypto();
```

###### Methods

| Method | Description |
| ---- | ----- |
| encrypt(int bit):string | Encrypts 1 or 0. Returns encrypted value as a base64 string. |
| decrypt(string encrypted):int | Decrypts. Returns 1 or 0. |
| export():object | Returns public key for use with Operator. |


##### Operator
```
var hcrypt = require('node-hcrypt');

var crypto = new hcrypt.Crypto();

var operator = new hcrypt.Operator(crypto.export());
```

###### Methods

| Method | Description |
| ---- | ----- |
| add(string encryptedA, string encryptedB):string encryptedC | Performs logical addition. |
| mul(string encryptedA, string encryptedB):string encryptedC | Performs logical multiplication. |
| halfAdd(string encryptedA, string encryptedB):\[string encryptedC, string carryD] | Performs half-addition with carry in return. |
| fullAdd(string carryI, string encryptedA, string encryptedB):\[string encryptedC, string carryD] | Performs full addition with a carry in return. |
| recrypt(string encryptedI):string encryptedO | Performs cryptographic refresh. Should be done after 2-3 operations under the same encrypted value. |


#### Tests

*WIP*

#### License

MIT
