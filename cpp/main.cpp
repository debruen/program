
#include "async.h"

// init Program class
Program program;

// -- -- -- -- -- Main

// Napi::Value program_main(const Napi::CallbackInfo& info) {
//   Napi::Function callback = info[0].As<Napi::Function>();
//
//   AsyncMain* main = new AsyncMain(callback, program);
//   main->Queue();
//
//   std::string msg = "program: main process started";
//   return Napi::String::New(info.Env(),msg.c_str());
// };

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

// -- -- -- -- -- Read

// Napi::Value program_read(const Napi::CallbackInfo& info) {
//   Napi::Uint8Array image_buffer = info[0].As<Napi::Uint8Array>();
//   Napi::Float32Array left_buffer = info[1].As<Napi::Float32Array>();
//   Napi::Float32Array right_buffer = info[2].As<Napi::Float32Array>();
//   std::size_t frame_index = info[3].ToNumber().Int64Value();
//   Napi::Function callback = info[4].As<Napi::Function>();
//
//   AsyncRead* read = new AsyncRead(callback, program, image_buffer, left_buffer, right_buffer, frame_index);
//   read->Queue();
//
//   std::string msg = "read queued";
//   return Napi::String::New(info.Env(),msg.c_str());
// };

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


// Napi::Value program_preview(const Napi::CallbackInfo& info) {
//   Napi::Uint8Array images_buffer = info[0].As<Napi::Uint8Array>();
//   Napi::Float32Array left_buffer = info[1].As<Napi::Float32Array>();
//   Napi::Float32Array right_buffer = info[2].As<Napi::Float32Array>();
//   Napi::Function callback = info[3].As<Napi::Function>();
//
//   AsyncPreview* preview = new AsyncPreview(callback, program, images_buffer, left_buffer, right_buffer);
//   preview->Queue();
//
//   std::string msg = "preview queued";
//   return Napi::String::New(info.Env(),msg.c_str());
// };

// Napi::Value program_save(const Napi::CallbackInfo& info) {
//   Napi::Function callback = info[0].As<Napi::Function>();
//
//   AsyncSave* save = new AsyncSave(callback, program);
//   save->Queue();
//
//   std::string msg = "save queued";
//   return Napi::String::New(info.Env(),msg.c_str());
// };

Napi::Object Init(Napi::Env env, Napi::Object exports) {

  // exports["program_main"]   = Napi::Function::New(env, program_main, std::string("program_main"));

  exports["program_data"]   = Napi::Function::New(env, program_data, std::string("program_data"));
  exports["program_update"] = Napi::Function::New(env, program_update, std::string("program_update"));

  // exports["program_read"]   = Napi::Function::New(env, program_read, std::string("program_read"));

  exports["program_buffer"]   = Napi::Function::New(env, program_buffer, std::string("program_buffer"));

  exports["program_quit"]   = Napi::Function::New(env, program_quit, std::string("program_quit"));

  // exports["program_preview"] = Napi::Function::New(env, program_preview, std::string("program_preview"));
  // exports["program_save"] = Napi::Function::New(env, program_save, std::string("program_save"));
  return exports;
}

NODE_API_MODULE(addon, Init)
