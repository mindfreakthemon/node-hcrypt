#ifndef OPERATOR_OBJECT_H
#define OPERATOR_OBJECT_H

#include <node.h>
#include <gmp.h>
#include "scarab.h"
#include "defines.h"

using namespace v8;

class Operator : public node::ObjectWrap {
 public:
  static void Init(Handle<Object> exports);

 private:
  explicit Operator(Handle<Object> o);
  ~Operator();

  static Handle<Value> New(const Arguments& args);
  static Handle<Value> Add(const Arguments& args);
  static Handle<Value> HalfAdd(const Arguments& args);
  static Handle<Value> FullAdd(const Arguments& args);
  static Handle<Value> Mul(const Arguments& args);
  static Persistent<Function> constructor;

  fhe_pk_t pk;
};

#endif
