#include "nodeprogram.h"

Napi::FunctionReference NodeProgram::constructor;

Napi::Object NodeProgram::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "NodeProgram", {

    InstanceMethod("data", &NodeProgram::WrapData),
    InstanceMethod("process", &NodeProgram::WrapProcess),

  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("NodeProgram", func);
  return exports;
}

NodeProgram::NodeProgram(const Napi::CallbackInfo& info) : Napi::ObjectWrap<NodeProgram>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  this->program_ = new Program();
}

Napi::Value NodeProgram::WrapData(const Napi::CallbackInfo& info) { // wrapped data
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  nlohmann::json data;

  if (info.Length() < 1) {
    data = this->program_->data();

  } else {

    Napi::Object json_object = info[0].As<Napi::Object>();
    Napi::Object json = env.Global().Get("JSON").As<Napi::Object>();
    Napi::Function stringify = json.Get("stringify").As<Napi::Function>();
    std::string dataIn = stringify.Call(json, { json_object }).As<Napi::String>();

    data = this->program_->data(nlohmann::json::parse(dataIn));
  }

  m_display_width  = data["width"];
  m_display_height = data["height"];
  m_frames = data["input"][1]["value"];

  std::string dataOut = data.dump();

  Napi::String json_string = Napi::String::New(env, dataOut);
  Napi::Object json = env.Global().Get("JSON").As<Napi::Object>();
  Napi::Function parse = json.Get("parse").As<Napi::Function>();

  return parse.Call(json, { json_string }).As<Napi::Object>();

}

// ------------------------------------------------------------------------- computation

Napi::Value NodeProgram::WrapProcess(const Napi::CallbackInfo& info) { // wrapped process
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  if (info.Length() < 1) {
    this->program_->process();

  } else {

    // receiving Buffer
    Napi::Uint8Array image_buffer = info[0].As<Napi::Uint8Array>();
    Napi::Float32Array audio_buffer_l = info[1].As<Napi::Float32Array>();
    Napi::Float32Array audio_buffer_r = info[2].As<Napi::Float32Array>();

    std::vector<cv::Mat> images;

    cv::Size size(m_display_width, m_display_height);
    for (int i = 0; i < m_frames; i++) {
      cv::Mat image = cv::Mat(size, CV_8UC3);
      images.push_back(image);
    }

    stk::StkFrames audio;

    // get audio buffer length
    unsigned int audio_buffer_length = audio_buffer_l.ByteLength() / sizeof(float);
    audio.resize(audio_buffer_length, 2);

    this->program_->process(images, audio);

    for (int i = 0; i < m_frames; i++) {
      cv::cvtColor(images[i], images[i], cv::COLOR_BGR2BGRA);
    }

    // writing result to image_buffer
    unsigned int p, c, z;
    uchar* ptr;

    for (int i = 0; i < m_frames; i++) {
      p = m_display_width * m_display_height * 4 * i;

      for (unsigned int y = 0; y < m_display_height; y++) {
        ptr = images[i].ptr<uchar>(y);

        for (unsigned int x = 0; x < m_display_width; x++) {
          z = (y * m_display_width + x) * 4 + p;
          c = x * 4;

          image_buffer[z]   = ptr[c+2]; // red
          image_buffer[z+1] = ptr[c+1]; // green
          image_buffer[z+2] = ptr[c];   // blue
          image_buffer[z+3] = ptr[c+3]; // alpha

        }

      }

    }

    // writing result to audio Buffer
    stk::StkFrames framesL(audio_buffer_length, 1), framesR(audio_buffer_length, 1);

    framesL.setChannel(0, audio, 0);
    framesR.setChannel(0, audio, 1);

    for(unsigned int i = 0; i < audio_buffer_length; i++) {

      audio_buffer_l[i] = framesL[i];
      audio_buffer_r[i] = framesR[i];
    }
  }

  return Napi::Boolean::New(env, true);
}
