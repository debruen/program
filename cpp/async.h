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
    nlohmann::json m_data;

  public:
    AsyncNewFrame(Napi::Function& callback, Program& program)
      : AsyncWorker(callback), program(program) {
    };
    virtual ~AsyncNewFrame() {};

    void Execute() {
      m_data = program.new_frame();
    };
    void OnOK() {
      std::string string = m_data.dump();
      Callback().Call({Env().Null(), Napi::String::New(Env(), string)});
    };
};

// -- -- -- -- -- display
class AsyncDisplay : public Napi::AsyncWorker {

  private:
    Program& program;
    nlohmann::json m_data;
    Napi::Uint8Array m_image_array, m_left_array, m_right_array;
    int m_image_width, m_image_height, m_audio_width, m_audio_height;
    cv::Mat m_image, m_left, m_right;

  public:
    AsyncDisplay(Napi::Function& callback, Program& program, nlohmann::json data, Napi::Uint8Array image, Napi::Uint8Array left, Napi::Uint8Array right)
      : AsyncWorker(callback), program(program), m_data(data), m_image_array(image), m_left_array(left), m_right_array(right) {

      m_image_width  = m_data["imageWidth"];
      m_image_height = m_data["imageHeight"];
      m_audio_width  = m_data["audioWidth"];
      m_audio_height = m_data["audioHeight"];

      m_image = cv::Mat::zeros(cv::Size(m_image_width, m_image_height), CV_8UC3);
      m_left  = cv::Mat::zeros(cv::Size(m_audio_width, m_audio_height), CV_8UC3);
      m_right = cv::Mat::zeros(cv::Size(m_audio_width, m_audio_height), CV_8UC3);
    };
    virtual ~AsyncDisplay() {};

    void Execute() {
      program.display(m_image, m_left, m_right);
    };
    void OnOK() {

      // writing result to image_buffer
      int z_image, c_image;
      uchar* image_ptr;
      for (int y = 0; y < m_image_height; y++) {

        image_ptr = m_image.ptr<uchar>(y);
        for (int x = 0; x < m_image_width; x++) {
          z_image = (y * m_image_width + x) * 4;
          c_image = x * 3;

          m_image_array[z_image]   = image_ptr[c_image+2]; // red
          m_image_array[z_image+1] = image_ptr[c_image+1]; // green
          m_image_array[z_image+2] = image_ptr[c_image];   // blue
          m_image_array[z_image+3] = 255;      // alpha channel
        }
      }

      // writing result to audio_buffer
      int z_audio, c_audio;
      uchar* left_ptr;
      uchar* right_ptr;
      for (int y = 0; y < m_audio_height; y++) {

        left_ptr = m_left.ptr<uchar>(y);
        right_ptr = m_right.ptr<uchar>(y);
        for (int x = 0; x < m_audio_width; x++) {
          z_audio = (y * m_audio_width + x) * 4;
          c_audio = x * 3;

          m_left_array[z_audio]   = left_ptr[c_audio+2]; // red
          m_left_array[z_audio+1] = left_ptr[c_audio+1]; // green
          m_left_array[z_audio+2] = left_ptr[c_audio];   // blue
          m_left_array[z_audio+3] = 255;      // alpha channel

          m_right_array[z_audio]   = right_ptr[c_audio+2]; // red
          m_right_array[z_audio+1] = right_ptr[c_audio+1]; // green
          m_right_array[z_audio+2] = right_ptr[c_audio];   // blue
          m_right_array[z_audio+3] = 255;      // alpha channel
        }
      }

      bool ready = true;
      Callback().Call({Env().Null(), Napi::Boolean::New(Env(), ready)});
    };
};


// -- -- -- -- -- record
class AsyncRecord : public Napi::AsyncWorker {

  private:
    Program& program;
    bool m_record = false;

  public:
    AsyncRecord(Napi::Function& callback, Program& program)
      : AsyncWorker(callback), program(program) {
    };
    virtual ~AsyncRecord() {};

    void Execute() {
      m_record = program.record();
    };
    void OnOK() {
      Callback().Call({Env().Null(), Napi::Boolean::New(Env(), m_record)});
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
