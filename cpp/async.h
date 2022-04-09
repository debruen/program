#ifndef async_h
#define async_h

#include <napi.h>

#include <chrono>
#include <thread>
#include <iomanip>

#include "src/program.h"

// -- -- -- -- -- init synthesis
class AsyncInitSynthesis : public Napi::AsyncWorker {

  private:
    Program& program;
    nlohmann::json m_data;

  public:
    AsyncInitSynthesis(Napi::Function& callback, Program& program)
      : AsyncWorker(callback), program(program) {
    };
    virtual ~AsyncInitSynthesis() {};

    void Execute() {
      m_data = program.init_synthesis();
    };
    void OnOK() {
      std::string string = m_data.dump();
      Callback().Call({Env().Null(), Napi::String::New(Env(), string)});
    };
};

// -- -- -- -- -- data synthesis
class AsyncDataSynthesis : public Napi::AsyncWorker {

  private:
    Program& program;
    nlohmann::json m_data;

  public:
    AsyncDataSynthesis(Napi::Function& callback, Program& program, nlohmann::json data)
      : AsyncWorker(callback), program(program), m_data(data) {
    };
    virtual ~AsyncDataSynthesis() {};

    void Execute() {
      m_data = program.data_synthesis(m_data);
    };
    void OnOK() {
      std::string string = m_data.dump();
      Callback().Call({Env().Null(), Napi::String::New(Env(), string)});
    };
};


// -- -- -- -- -- init control
class AsyncInitControl : public Napi::AsyncWorker {

  private:
    Program& program;
    nlohmann::json m_data;

  public:
    AsyncInitControl(Napi::Function& callback, Program& program)
      : AsyncWorker(callback), program(program) {
    };
    virtual ~AsyncInitControl() {};

    void Execute() {
      m_data = program.init_control();
    };
    void OnOK() {
      std::string string = m_data.dump();
      Callback().Call({Env().Null(), Napi::String::New(Env(), string)});
    };
};

// -- -- -- -- -- data control
class AsyncDataControl : public Napi::AsyncWorker {

  private:
    Program& program;
    nlohmann::json m_data;

  public:
    AsyncDataControl(Napi::Function& callback, Program& program, nlohmann::json data)
      : AsyncWorker(callback), program(program), m_data(data) {
    };
    virtual ~AsyncDataControl() {};

    void Execute() {
      m_data = program.data_control(m_data);
    };
    void OnOK() {
      std::string string = m_data.dump();
      Callback().Call({Env().Null(), Napi::String::New(Env(), string)});
    };
};


// -- -- -- -- -- new frame
class AsyncNewFrame : public Napi::AsyncWorker {

  private:
    Program& program;
    bool m_new = false;

  public:
    AsyncNewFrame(Napi::Function& callback, Program& program)
      : AsyncWorker(callback), program(program) {
    };
    virtual ~AsyncNewFrame() {};

    void Execute() {
      m_new = program.new_frame();
    };
    void OnOK() {
      Callback().Call({Env().Null(), Napi::Boolean::New(Env(), m_new)});
    };
};

// -- -- -- -- -- display
class AsyncDisplay : public Napi::AsyncWorker {

  private:
    Program& program;
    nlohmann::json m_data;
    Napi::Uint8Array p_image;
    int m_width, m_height;
    cv::Mat m_image;

  public:
    AsyncDisplay(Napi::Function& callback, Program& program, nlohmann::json data, Napi::Uint8Array image)
      : AsyncWorker(callback), program(program), m_data(data), p_image(image) {

      m_width  = m_data["width"];
      m_height = m_data["height"];

      cv::Size size(m_width, m_height);

      m_image = cv::Mat::zeros(cv::Size(m_width, m_height), CV_8UC3);
    };
    virtual ~AsyncDisplay() {};

    void Execute() {
      program.display(m_image);
    };
    void OnOK() {

      // writing result to image_buffer
      int c, z;
      uchar* image_ptr;

      for (int y = 0; y < m_height; y++) {

        image_ptr = m_image.ptr<uchar>(y);
        for (int x = 0; x < m_width; x++) {
          z = (y * m_width + x) * 4;
          c = x * 3;

          p_image[z]   = image_ptr[c+2]; // red
          p_image[z+1] = image_ptr[c+1]; // green
          p_image[z+2] = image_ptr[c];   // blue
          p_image[z+3] = 255;      // alpha channel
        }
      }

      bool ready = true;
      Callback().Call({Env().Null(), Napi::Boolean::New(Env(), ready)});
    };
};


// -- -- -- -- -- quit
class AsyncQuit : public Napi::AsyncWorker {

  private:
    Program& program;
    bool m_quit = false;

  public:
    AsyncQuit(Napi::Function& callback, Program& program)
      : AsyncWorker(callback), program(program) {
    };
    virtual ~AsyncQuit() {};

    void Execute() {
      m_quit = program.quit();
    };
    void OnOK() {
      Callback().Call({Env().Null(), Napi::Boolean::New(Env(), m_quit)});
    };
};

#endif
