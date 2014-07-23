#ifndef SCARAB_OBJECT_H
#define SCARAB_OBJECT_H

#include <node.h>
#include <gmp.h>
#include "scarab.h"

using namespace v8;

class Scarab : public node::ObjectWrap {
 public:
  static void Init(Handle<Object> exports);

 private:
  explicit Scarab();
  ~Scarab();

  static Handle<Value> New(const Arguments& args);
  static Handle<Value> Encrypt(const Arguments& args);
  static Handle<Value> Decrypt(const Arguments& args);
//  static Handle<Value> Recrypt(const Arguments& args);
//  static Handle<Value> Add(const Arguments& args);
//  static Handle<Value> HalfAdd(const Arguments& args);
//  static Handle<Value> FullAdd(const Arguments& args);
//  static Handle<Value> Mul(const Arguments& args);
  static Persistent<Function> constructor;

  fhe_pk_t pk;
  fhe_sk_t sk;
};

#endif
