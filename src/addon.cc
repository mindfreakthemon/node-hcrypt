#include <nan.h>
#include "crypto_object.h"
#include "operator_object.h"

using v8::FunctionTemplate;

NAN_MODULE_INIT(InitAll) {
  Crypto::Init(target);
  Operator::Init(target);
}

NODE_MODULE(addon, InitAll)
