
#include "async.h"

// init Program class
Program program;

// AsyncInit

Napi::Value program_init(const Napi::CallbackInfo& info) {

  // Parameter info[]
  Napi::Function callback = info[0].As<Napi::Function>();

  // init new AsyncInit
  AsyncInit* init = new AsyncInit(callback, program);

  // Start AsyncWorker
  init->Queue();

  // return
  std::string msg = "init queued";

  return Napi::String::New(info.Env(),msg.c_str());
};

// async data
Napi::Value program_update(const Napi::CallbackInfo& info) {

  // Parameter info[]

  std::string string = info[0].As<Napi::String>().Utf8Value();
  Napi::Function callback = info[1].As<Napi::Function>();

  nlohmann::json json = nlohmann::json::parse(string);

  // init new AsyncUpdate
  AsyncUpdate* update = new AsyncUpdate(callback, program, json);

  // Start AsyncWorker
  update->Queue();

  // return
  std::string msg = "data queued";

  return Napi::String::New(info.Env(),msg.c_str());
};

// async preview
Napi::Value program_preview(const Napi::CallbackInfo& info) {

  // Parameter info[]
  // receiving Buffer
  Napi::Uint8Array images_buffer = info[0].As<Napi::Uint8Array>();
  Napi::Float32Array left_buffer = info[1].As<Napi::Float32Array>();
  Napi::Float32Array right_buffer = info[2].As<Napi::Float32Array>();

  Napi::Function callback = info[3].As<Napi::Function>();

  // uchar *images = reinterpret_cast<uchar *>(images_buffer.ArrayBuffer().Data());
  // float *left = reinterpret_cast<float *>(left_buffer.ArrayBuffer().Data());
  // float *right = reinterpret_cast<float *>(right_buffer.ArrayBuffer().Data());


  // init new AsyncPreview
  AsyncPreview* preview = new AsyncPreview(callback, program, images_buffer, left_buffer, right_buffer);

  // Start AsyncWorker
  preview->Queue();

  // return
  std::string msg = "preview queued";

  return Napi::String::New(info.Env(),msg.c_str());
};

Napi::Value program_save(const Napi::CallbackInfo& info) {

  Napi::Function callback = info[0].As<Napi::Function>();

  AsyncSave* save = new AsyncSave(callback, program);

  // Start AsyncWorker
  save->Queue();

  // return
  std::string msg = "save queued";

  return Napi::String::New(info.Env(),msg.c_str());
};

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports["program_init"] = Napi::Function::New(env, program_init, std::string("program_init"));
  exports["program_update"] = Napi::Function::New(env, program_update, std::string("program_update"));
  exports["program_preview"] = Napi::Function::New(env, program_preview, std::string("program_preview"));
  exports["program_save"] = Napi::Function::New(env, program_save, std::string("program_save"));
  return exports;
}

NODE_API_MODULE(addon, Init)
