#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <node.h>

#include "scarab.h"


class MyObject : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  explicit MyObject(double value = 0);
  ~MyObject();

  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> PlusOne(const v8::Arguments& args);
  static v8::Persistent<v8::Function> constructor;
  double value_;

  fhe_pk_t pk;
  fhe_sk_t sk;
};

#endif
