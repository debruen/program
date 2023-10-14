
#include <napi.h>
#include "nodeclass.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return NodeIA::Init(env, exports);
}

NODE_API_MODULE(module, InitAll)
