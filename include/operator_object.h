#ifndef OPERATOR_OBJECT_H
#define OPERATOR_OBJECT_H

#include <nan.h>
#include "scarab.h"
#include "defines.h"

class Operator : public Nan::ObjectWrap {
public:
    static NAN_MODULE_INIT(Init);

private:
    explicit Operator(v8::Local<v8::Object> o);
    ~Operator();

    static NAN_METHOD(New);
    static NAN_METHOD(Add);
    static NAN_METHOD(HalfAdd);
    static NAN_METHOD(FullAdd);
    static NAN_METHOD(Mul);
    static NAN_METHOD(Recrypt);
    static Nan::Persistent<v8::Function> constructor;

    fhe_pk_t pk;
};

#endif
