
#include "async.h"

// init Program class
Program program;

// -- -- -- -- -- Data

Napi::Value program_data(const Napi::CallbackInfo& info) {
  Napi::Function callback = info[0].As<Napi::Function>();

  AsyncData* data = new AsyncData(callback, program);
  data->Queue();

  std::string msg = "program: reading data in progress";
  return Napi::String::New(info.Env(),msg.c_str());
};

// -- -- -- -- -- Update

Napi::Value program_update(const Napi::CallbackInfo& info) {
  std::string string = info[0].As<Napi::String>().Utf8Value();
  Napi::Function callback = info[1].As<Napi::Function>();

  nlohmann::json json = nlohmann::json::parse(string);

  AsyncUpdate* update = new AsyncUpdate(callback, program, json);
  update->Queue();

  std::string msg = "program: update data in progress";
  return Napi::String::New(info.Env(),msg.c_str());
};

// -- -- -- -- -- Buffer

Napi::Value program_buffer(const Napi::CallbackInfo& info) {

  std::string string = info[0].As<Napi::String>().Utf8Value();
  Napi::Uint8Array image_buffer = info[1].As<Napi::Uint8Array>();
  Napi::Function callback = info[2].As<Napi::Function>();

  nlohmann::json json = nlohmann::json::parse(string);

  AsyncBuffer* read = new AsyncBuffer(callback, program, json, image_buffer);
  read->Queue();

  std::string msg = "read queued";
  return Napi::String::New(info.Env(),msg.c_str());
};

// -- -- -- -- -- Quit

Napi::Value program_quit(const Napi::CallbackInfo& info) {
  Napi::Function callback = info[0].As<Napi::Function>();

  AsyncQuit* quit = new AsyncQuit(callback, program);
  quit->Queue();

  std::string msg = "program: quit in progress";
  std::cout << msg << '\n';
  return Napi::String::New(info.Env(),msg.c_str());
};

Napi::Object Init(Napi::Env env, Napi::Object exports) {

  exports["program_data"] = Napi::Function::New(env, program_data, std::string("program_data"));
  exports["program_update"] = Napi::Function::New(env, program_update, std::string("program_update"));
  exports["program_buffer"] = Napi::Function::New(env, program_buffer, std::string("program_buffer"));
  exports["program_quit"] = Napi::Function::New(env, program_quit, std::string("program_quit"));

  return exports;
}

NODE_API_MODULE(addon, Init)
