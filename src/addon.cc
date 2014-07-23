#include <node.h>
#include "crypto_object.h"
#include "operator_object.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
  Crypto::Init(exports);
  Operator::Init(exports);
}

NODE_MODULE(addon, InitAll)
