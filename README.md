hcrypt
===========
### Fully homomorphic encryption implementation for node.js

This package provides an API for node.js application to use homomorphic encryption to perform some computations.

Only integers are currently supported.

Operations supported: algebraic addition and multiplication.

----

#### Prerequisites

You must have GMP (5.0.0+ or MPIR 2.6.0+) and MPFR (3.0.0+) libraries installed in the system.
Also, build tools are required for the native addon.

For Debian-based: ```sudo apt-get install libgmp-dev libmpfr-dev libmpfr4 gcc g++ automake autoconf```

For RHEL-based: ```sudo yum install gmp mpfr gcc gcc-c++ automake autoconf```, but note that gmp and mpfr versions in the repo are outdated so you should just compile them from sources.

On Windows you can use cygwin, just don't forget to set GMP and MPFR to *devel* while installing.

This package should work with latest stable npm and node, but may also work with any below. 
If you run into any problems while installing be sure to udpate npm first.

MPFR: http://www.mpfr.org/mpfr-current/#download
GMP: https://gmplib.org/#DOWNLOAD

---

#### To Install

```npm install node-hcrypt```

The installation may take a while.

----

#### API

The module exports two main object types: Crypto and Operator. 
Crypto provides with encrypting / decrypting functionality while Operator deals with the operations on encrypted data.
Module also exports 

##### Crypto
```
var hcrypt = require('node-hcrypt');

var crypto = new hcrypt.Crypto();
```

###### Methods

| Method | Description |
| ---- | ----- |
| encrypt(int bit):void | Encrypts 1 or 0. Returns encrypted value. |
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
| halfAdd(string encryptedA, string encryptedB):\[string encryptedC, string carriageD] | Performs half-addition with carriege return. |
| fullAdd(string carriageI, string encryptedA, string encryptedB):\[string encryptedC, string carriegeD] | Performs full addition with carriege return. |
| recrypt(string encryptedI):string encryptedO | Performs cryptographic refresh. Should be done after 2-3 operations under the same encrypted value. |


#### Tests

*WIP*

#### License

MIT