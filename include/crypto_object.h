#ifndef CRYPTO_OBJECT_H
#define CRYPTO_OBJECT_H

#include <node.h>
#include <gmp.h>
#include "scarab.h"
#include "defines.h"

using namespace v8;

class Crypto : public node::ObjectWrap {
 public:
  static void Init(Handle<Object> exports);

 private:
  explicit Crypto();
  ~Crypto();

  static Handle<Value> New(const Arguments& args);
  static Handle<Value> Encrypt(const Arguments& args);
  static Handle<Value> Decrypt(const Arguments& args);
  static Handle<Value> Export(const Arguments& args);
  static Persistent<Function> constructor;

  fhe_pk_t pk;
  fhe_sk_t sk;
};

#endif
