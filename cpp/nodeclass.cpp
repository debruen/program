#include "nodeclass.h"

Napi::FunctionReference NodeIA::constructor;

Napi::Object NodeIA::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "NodeIA", {

    InstanceMethod("set_data", &NodeIA::WrapSetData),
    InstanceMethod("get_data", &NodeIA::WrapGetData),

    // ------------------------------------------------------------------------- main settings

    InstanceMethod("set_mode", &NodeIA::WrapSetMode),
    InstanceMethod("get_mode", &NodeIA::WrapGetMode),

    // ------------------------------------------------------------------------- in settings

    InstanceMethod("set_image_in", &NodeIA::WrapSetImageIn),
    InstanceMethod("get_image_in", &NodeIA::WrapGetImageIn),

    InstanceMethod("add_image_layer", &NodeIA::WrapAddImageLayer),
    InstanceMethod("rmv_image_layer", &NodeIA::WrapRmvImageLayer),

    InstanceMethod("add_image_filter", &NodeIA::WrapAddImageFilter),
    InstanceMethod("rmv_image_filter", &NodeIA::WrapRmvImageFilter),

    InstanceMethod("set_audio_in", &NodeIA::WrapSetAudioIn),
    InstanceMethod("get_audio_in", &NodeIA::WrapGetAudioIn),

    InstanceMethod("add_audio_track", &NodeIA::WrapAddAudioTrack),
    InstanceMethod("rmv_audio_track", &NodeIA::WrapRmvAudioTrack),

    InstanceMethod("add_audio_filter", &NodeIA::WrapAddAudioFilter),
    InstanceMethod("rmv_audio_filter", &NodeIA::WrapRmvAudioFilter),

    // ---------------------------------------------------------------------------- out settings

    InstanceMethod("set_image_out", &NodeIA::WrapSetImageOut),
    InstanceMethod("get_image_out", &NodeIA::WrapGetImageOut),

    InstanceMethod("set_audio_out", &NodeIA::WrapSetAudioOut),
    InstanceMethod("get_audio_out", &NodeIA::WrapGetAudioOut),

    InstanceMethod("get_out_pages", &NodeIA::WrapGetOutPages),
    InstanceMethod("get_out_time", &NodeIA::WrapGetOutTime),
    // ------------------------------------------------------------------------- computation

    InstanceMethod("preview", &NodeIA::WrapPreview),
    InstanceMethod("run", &NodeIA::WrapRun)

  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("NodeIA", func);
  return exports;
}

// wrapped Program() constructor
NodeIA::NodeIA(const Napi::CallbackInfo& info) : Napi::ObjectWrap<NodeIA>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  Napi::Object json_object = info[0].As<Napi::Object>();
  Napi::Object json = env.Global().Get("JSON").As<Napi::Object>();
  Napi::Function stringify = json.Get("stringify").As<Napi::Function>();

  std::string data = stringify.Call(json, { json_object }).As<Napi::String>();

  this->ia_ = new Program(data);
}

Napi::Value NodeIA::WrapSetData(const Napi::CallbackInfo& info) { // wrapped set_data
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  Napi::Object json_object = info[0].As<Napi::Object>();
  Napi::Object json = env.Global().Get("JSON").As<Napi::Object>();
  Napi::Function stringify = json.Get("stringify").As<Napi::Function>();

  std::string data = stringify.Call(json, { json_object }).As<Napi::String>();

  bool bol = this->ia_->set_data(data);
  return Napi::Boolean::New(env, bol);
}
Napi::Value NodeIA::WrapGetData(const Napi::CallbackInfo& info) { // wrapped get_data
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  std::string data = this->ia_->get_data();

  Napi::String json_string = Napi::String::New(env, data);
  Napi::Object json = env.Global().Get("JSON").As<Napi::Object>();
  Napi::Function parse = json.Get("parse").As<Napi::Function>();

  return parse.Call(json, { json_string }).As<Napi::Object>();
}

// ----------------------------------------------------------------------------- main settings

Napi::Value NodeIA::WrapSetMode(const Napi::CallbackInfo& info) { // wrapped set_mode
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  std::string mode = info[0].As<Napi::String>().Utf8Value();
  bool bol = this->ia_->set_mode(mode);
  return Napi::Boolean::New(env, bol);
}
Napi::Value NodeIA::WrapGetMode(const Napi::CallbackInfo& info) { // wrapped get_mode
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  std::string mode = this->ia_->get_mode();
  return Napi::String::New(env, mode);
}

// ----------------------------------------------------------------------------- in settings

Napi::Value NodeIA::WrapSetImageIn(const Napi::CallbackInfo& info) { // wrapped set_image_mode
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  std::string in = info[0].As<Napi::String>().Utf8Value();
  bool bol = this->ia_->set_image_in(in);
  return Napi::Boolean::New(env, bol);
}
Napi::Value NodeIA::WrapGetImageIn(const Napi::CallbackInfo& info) { // wrapped get_image_mode
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  std::string in = this->ia_->get_image_in();
  return Napi::String::New(env, in);
}

Napi::Value NodeIA::WrapAddImageLayer(const Napi::CallbackInfo& info) { // wrapped addLayer
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  bool bol = this->ia_->add_image_layer();
  return Napi::Boolean::New(env, bol);
}
Napi::Value NodeIA::WrapRmvImageLayer(const Napi::CallbackInfo& info) { // wrapped removeLayer
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  bool bol = this->ia_->rmv_image_layer();
  return Napi::Boolean::New(env, bol);
}

Napi::Value NodeIA::WrapAddImageFilter(const Napi::CallbackInfo& info) { // wrapped add_image_filter_num
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  bool bol = this->ia_->add_image_filter();
  return Napi::Boolean::New(env, bol);
}
Napi::Value NodeIA::WrapRmvImageFilter(const Napi::CallbackInfo& info) { // wrapped rmv_image_filter_num
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  bool bol = this->ia_->rmv_image_filter();
  return Napi::Boolean::New(env, bol);
}

Napi::Value NodeIA::WrapSetAudioIn(const Napi::CallbackInfo& info) { // wrapped set_image_mode
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  std::string in = info[0].As<Napi::String>().Utf8Value();
  bool bol = this->ia_->set_audio_in(in);
  return Napi::Boolean::New(env, bol);
}
Napi::Value NodeIA::WrapGetAudioIn(const Napi::CallbackInfo& info) { // wrapped get_image_mode
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  std::string in = this->ia_->get_audio_in();
  return Napi::String::New(env, in);
}

Napi::Value NodeIA::WrapAddAudioTrack(const Napi::CallbackInfo& info) { // wrapped add_audiosynth_track
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  bool bol = this->ia_->add_audio_track();
  return Napi::Boolean::New(env, bol);
}
Napi::Value NodeIA::WrapRmvAudioTrack(const Napi::CallbackInfo& info) { // wrapped remove_audiosynth_track
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  bool bol = this->ia_->rmv_audio_track();
  return Napi::Boolean::New(env, bol);
}

Napi::Value NodeIA::WrapAddAudioFilter(const Napi::CallbackInfo& info) { // wrapped add_audiosynth_filter
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  bool bol = this->ia_->add_audio_filter();
  return Napi::Boolean::New(env, bol);
}
Napi::Value NodeIA::WrapRmvAudioFilter(const Napi::CallbackInfo& info) { // wrapped remove_audiosynth_filter
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  bool bol = this->ia_->rmv_audio_filter();
  return Napi::Boolean::New(env, bol);
}

// ----------------------------------------------------------------------------- out

Napi::Value NodeIA::WrapSetImageOut(const Napi::CallbackInfo& info) { // wrapped set_image_mode
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  std::string out = info[0].As<Napi::String>().Utf8Value();
  bool bol = this->ia_->set_image_out(out);
  return Napi::Boolean::New(env, bol);
}
Napi::Value NodeIA::WrapGetImageOut(const Napi::CallbackInfo& info) { // wrapped get_image_mode
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  std::string out = this->ia_->get_image_out();
  return Napi::String::New(env, out);
}

Napi::Value NodeIA::WrapSetAudioOut(const Napi::CallbackInfo& info) { // wrapped set_image_mode
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  std::string out = info[0].As<Napi::String>().Utf8Value();
  bool bol = this->ia_->set_audio_out(out);
  return Napi::Boolean::New(env, bol);
}
Napi::Value NodeIA::WrapGetAudioOut(const Napi::CallbackInfo& info) { // wrapped get_image_mode
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  std::string out = this->ia_->get_audio_out();
  return Napi::String::New(env, out);
}

Napi::Value NodeIA::WrapGetOutPages(const Napi::CallbackInfo& info) { // wrapped get_out_pages
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  unsigned int pages = this->ia_->get_out_pages();
  return Napi::Number::New(env, pages);
}
Napi::Value NodeIA::WrapGetOutTime(const Napi::CallbackInfo& info) { // wrapped get_out_time
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  double time = this->ia_->get_out_time();
  return Napi::Number::New(env, time);
}
// ------------------------------------------------------------------------- computation

Napi::Value NodeIA::WrapPreview(const Napi::CallbackInfo& info) { // wrapped preview
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  Napi::Uint8Array buffer = info[0].As<Napi::Uint8Array>();

  Napi::Float32Array audioL = info[1].As<Napi::Float32Array>();
  Napi::Float32Array audioR = info[2].As<Napi::Float32Array>();

  unsigned int width = info[3].As<Napi::Number>().Uint32Value();
  unsigned int height = info[4].As<Napi::Number>().Uint32Value();
  unsigned int pages = info[5].As<Napi::Number>().Uint32Value();

  bool audioPreview = info[6].As<Napi::Boolean>().Value();

  std::vector< std::vector< std::vector< std::vector<unsigned char> > > > vector;

  unsigned int p, i;

  for (unsigned int c = 0; c < pages; c++) {
    std::vector< std::vector< std::vector<unsigned char> > > tempI;

    p = width * height * 4 * c;

    for (unsigned int y = 0; y < height; y++) {
      std::vector< std::vector<unsigned char> > tempY;
      for (unsigned int x = 0; x < width; x++) {
        std::vector<unsigned char> tempX;
        i = (y * width + x) * 4 + p;

        tempX.push_back(buffer[i]);
        tempX.push_back(buffer[i + 1]);
        tempX.push_back(buffer[i + 2]);
        tempX.push_back(buffer[i + 3]);

        tempY.push_back(tempX);
      }
      tempI.push_back(tempY);
    }
    vector.push_back(tempI);

  }

  std::vector<double> vectorL, vectorR;
  unsigned int length = audioL.ElementLength();
  for (unsigned int i = 0; i < length; i++) {
    vectorL.push_back(audioL[i]);
    vectorR.push_back(audioR[i]);
  }

  this->ia_->preview(vector, vectorL, vectorR, audioPreview);

  for (unsigned int c = 0; c < pages; c++) {
    p = width * height * 4 * c;
    for (unsigned int y = 0; y < height; y++) {
      for (unsigned int x = 0; x < width; x++) {
        i = (y * width + x) * 4 + p;
        buffer[i  ] = vector[c][y][x][0];   // red
        buffer[i+1] = vector[c][y][x][1];   // green
        buffer[i+2] = vector[c][y][x][2];   // blue
        buffer[i+3] = 255;   // alpha
      }
    }
  }

  for (unsigned int i = 0; i < length; i++) {
    audioL[i] = vectorL[i];
    audioR[i] = vectorR[i];
  }

  return Napi::Boolean::New(env, true);
}

Napi::Value NodeIA::WrapRun(const Napi::CallbackInfo& info) { // wrapped run
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  bool bol = this->ia_->run();

  return Napi::Boolean::New(env, bol);
}
