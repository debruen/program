
#include "async.h"

// init Program class
Program program;

Napi::Value program_main(const Napi::CallbackInfo& info) {
  Napi::Function callback = info[0].As<Napi::Function>();

  AsyncWork* main = new AsyncWork(callback, program);
  main->Queue();

  std::string msg = "program: main process started";
  return Napi::String::New(info.Env(),msg.c_str());
};

Napi::Value program_data(const Napi::CallbackInfo& info) {
  Napi::Function callback = info[0].As<Napi::Function>();

  AsyncInit* data = new AsyncInit(callback, program);
  data->Queue();

  std::string msg = "program: reading data in progress";
  return Napi::String::New(info.Env(),msg.c_str());
};

Napi::Value program_update(const Napi::CallbackInfo& info) {
  std::string string = info[0].As<Napi::String>().Utf8Value();
  Napi::Function callback = info[1].As<Napi::Function>();

  nlohmann::json json = nlohmann::json::parse(string);

  AsyncUpdate* update = new AsyncUpdate(callback, program, json);
  update->Queue();

  std::string msg = "program: update data in progress";
  return Napi::String::New(info.Env(),msg.c_str());
};

Napi::Value program_quit(const Napi::CallbackInfo& info) {
  Napi::Function callback = info[0].As<Napi::Function>();

  AsyncQuit* quit = new AsyncQuit(callback, program);
  quit->Queue();

  std::string msg = "program: quit in progress";
  std::cout << msg << '\n';
  return Napi::String::New(info.Env(),msg.c_str());
};


Napi::Value program_preview(const Napi::CallbackInfo& info) {
  Napi::Uint8Array images_buffer = info[0].As<Napi::Uint8Array>();
  Napi::Float32Array left_buffer = info[1].As<Napi::Float32Array>();
  Napi::Float32Array right_buffer = info[2].As<Napi::Float32Array>();
  Napi::Function callback = info[3].As<Napi::Function>();

  AsyncPreview* preview = new AsyncPreview(callback, program, images_buffer, left_buffer, right_buffer);
  preview->Queue();

  std::string msg = "preview queued";
  return Napi::String::New(info.Env(),msg.c_str());
};

Napi::Value program_save(const Napi::CallbackInfo& info) {
  Napi::Function callback = info[0].As<Napi::Function>();

  AsyncSave* save = new AsyncSave(callback, program);
  save->Queue();

  std::string msg = "save queued";
  return Napi::String::New(info.Env(),msg.c_str());
};

Napi::Object Init(Napi::Env env, Napi::Object exports) {

  exports["program_main"]   = Napi::Function::New(env, program_main, std::string("program_main"));

  exports["program_data"]   = Napi::Function::New(env, program_data, std::string("program_data"));
  exports["program_update"] = Napi::Function::New(env, program_update, std::string("program_update"));

  exports["program_quit"]   = Napi::Function::New(env, program_quit, std::string("program_quit"));


  exports["program_preview"] = Napi::Function::New(env, program_preview, std::string("program_preview"));
  exports["program_save"] = Napi::Function::New(env, program_save, std::string("program_save"));
  return exports;
}

NODE_API_MODULE(addon, Init)
