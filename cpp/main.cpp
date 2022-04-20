
#include "async.h"

// init Program class
Program program;

// -- -- -- -- -- init synthesis
Napi::Value program_init_synthesis(const Napi::CallbackInfo& info) {
  Napi::Function callback = info[0].As<Napi::Function>();

  AsyncInitSynthesis* init_synthesis = new AsyncInitSynthesis(callback, program);
  init_synthesis->Queue();

  std::string msg = "program: init synthesis";
  return Napi::String::New(info.Env(),msg.c_str());
};

// -- -- -- -- -- data synthesis
Napi::Value program_data_synthesis(const Napi::CallbackInfo& info) {
  std::string string = info[0].As<Napi::String>().Utf8Value();
  Napi::Function callback = info[1].As<Napi::Function>();

  nlohmann::json json = nlohmann::json::parse(string);

  AsyncDataSynthesis* data_synthesis = new AsyncDataSynthesis(callback, program, json);
  data_synthesis->Queue();

  std::string msg = "program: data synthesis";
  return Napi::String::New(info.Env(),msg.c_str());
};


// -- -- -- -- -- init control
Napi::Value program_init_control(const Napi::CallbackInfo& info) {
  Napi::Function callback = info[0].As<Napi::Function>();

  AsyncInitControl* init_control = new AsyncInitControl(callback, program);
  init_control->Queue();

  std::string msg = "program: init control";
  return Napi::String::New(info.Env(),msg.c_str());
};

// -- -- -- -- -- data control
Napi::Value program_data_control(const Napi::CallbackInfo& info) {
  std::string string = info[0].As<Napi::String>().Utf8Value();
  Napi::Function callback = info[1].As<Napi::Function>();

  nlohmann::json json = nlohmann::json::parse(string);

  AsyncDataControl* data_control = new AsyncDataControl(callback, program, json);
  data_control->Queue();

  std::string msg = "program: data control";
  return Napi::String::New(info.Env(),msg.c_str());
};


// -- -- -- -- -- new frame
Napi::Value program_new_frame(const Napi::CallbackInfo& info) {
  Napi::Function callback = info[0].As<Napi::Function>();

  AsyncNewFrame* new_frame = new AsyncNewFrame(callback, program);
  new_frame->Queue();

  std::string msg = "program: new frame";
  return Napi::String::New(info.Env(),msg.c_str());
};

// -- -- -- -- -- display
Napi::Value program_display(const Napi::CallbackInfo& info) {

  std::string string = info[0].As<Napi::String>().Utf8Value();
  Napi::Uint8Array image_buffer = info[1].As<Napi::Uint8Array>();
  Napi::Function callback = info[2].As<Napi::Function>();

  nlohmann::json json = nlohmann::json::parse(string);

  AsyncDisplay* display = new AsyncDisplay(callback, program, json, image_buffer);
  display->Queue();

  std::string msg = "display";
  return Napi::String::New(info.Env(),msg.c_str());
};


// -- -- -- -- -- record
Napi::Value program_record(const Napi::CallbackInfo& info) {
  std::cout << "main cpp record" << '\n';
  Napi::Function callback = info[0].As<Napi::Function>();

  AsyncRecord* record = new AsyncRecord(callback, program);
  record->Queue();

  std::string msg = "program: record in progress";
  return Napi::String::New(info.Env(),msg.c_str());
};

// -- -- -- -- -- quit
Napi::Value program_quit(const Napi::CallbackInfo& info) {
  Napi::Function callback = info[0].As<Napi::Function>();

  AsyncQuit* quit = new AsyncQuit(callback, program);
  quit->Queue();

  std::string msg = "program: quit in progress";
  return Napi::String::New(info.Env(),msg.c_str());
};

Napi::Object Init(Napi::Env env, Napi::Object exports) {

  exports["program_init_synthesis"] = Napi::Function::New(env, program_init_synthesis, std::string("program_init_synthesis"));
  exports["program_data_synthesis"] = Napi::Function::New(env, program_data_synthesis, std::string("program_data_synthesis"));

  exports["program_init_control"] = Napi::Function::New(env, program_init_control, std::string("program_init_control"));
  exports["program_data_control"] = Napi::Function::New(env, program_data_control, std::string("program_data_control"));

  exports["program_new_frame"] = Napi::Function::New(env, program_new_frame, std::string("program_new_frame"));
  exports["program_display"] = Napi::Function::New(env, program_display, std::string("program_display"));
  exports["program_record"] = Napi::Function::New(env, program_record, std::string("program_record"));

  exports["program_quit"] = Napi::Function::New(env, program_quit, std::string("program_quit"));

  return exports;
}

NODE_API_MODULE(addon, Init)
