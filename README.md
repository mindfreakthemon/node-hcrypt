hcrypt
===========
### Fully homomorphic encryption implementation for node.js

This package provides an API for node.js application to use homomorphic encryption to perform computations.

Only integers are currently supported.

Operations supported: algebraic addition and multiplication.

----

#### Prerequisites

You must have GMP (5.0.0+ or MPIR 2.6.0+) and MPFR (3.0.0+) libraries installed in the system.

##### Debian-based
```sudo apt-get install libgmp-dev libmpfr-dev libmpfr4 gcc g++ automake autoconf```.

##### RHEL-based
```sudo yum install gcc gcc-c++ automake autoconf```.
GMP and MPFR versions in the repo are outdated so you should just compile them from sources. Look for download links below.


##### Windows
Use cygwin, just don't forget to set GMP and MPFR to *devel* while installing.

This package should work with latest stable npm and node, but may also work with any below. 
If you run into any problems while installing be sure to udpate npm first.

#### Downloads
* MPFR: http://www.mpfr.org/mpfr-current/#download
* GMP: https://gmplib.org/#DOWNLOAD

---

#### To Install

```npm install node-hcrypt```

The installation may take awhile.

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
