#ifndef CRYPTO_OBJECT_H
#define CRYPTO_OBJECT_H

#include <nan.h>
#include "scarab.h"
#include "defines.h"

class Crypto : public Nan::ObjectWrap {
public:
    static NAN_MODULE_INIT(Init);

private:
    explicit Crypto();
    ~Crypto();

    static NAN_METHOD(New);
    static NAN_METHOD(Encrypt);
    static NAN_METHOD(Decrypt);
    static NAN_METHOD(Recrypt);
    static NAN_METHOD(Export);
    static Nan::Persistent<v8::Function> constructor;

    fhe_pk_t pk;
    fhe_sk_t sk;
};

#endif
