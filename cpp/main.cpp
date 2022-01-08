
#include <napi.h>
#include "nodeprogram.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return NodeProgram::Init(env, exports);
}

NODE_API_MODULE(module, InitAll)
