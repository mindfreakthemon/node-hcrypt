#include <node.h>
#include "scarab_object.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
  Scarab::Init(exports);
}

NODE_MODULE(addon, InitAll)
