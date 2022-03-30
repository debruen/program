#ifndef async_h
#define async_h

#include <napi.h>

#include <chrono>
#include <thread>
#include <iomanip>

#include "src/program.h"

// -- -- -- -- -- Data

class AsyncData : public Napi::AsyncWorker {

  private:

    Program& program;
    nlohmann::json m_data;

  public:
    AsyncData(Napi::Function& callback, Program& program)
      : AsyncWorker(callback), program(program) {
    };
    virtual ~AsyncData() {};

    void Execute() {
      m_data = program.data();
    };
    void OnOK() {
      std::string string = m_data.dump();
      Callback().Call({Env().Null(), Napi::String::New(Env(), string)});
    };
};

// -- -- -- -- -- Update

class AsyncUpdate : public Napi::AsyncWorker {

  private:

    Program& program;
    nlohmann::json m_data;

  public:
    AsyncUpdate(Napi::Function& callback, Program& program, nlohmann::json data)
      : AsyncWorker(callback), program(program), m_data(data) {
    };
    virtual ~AsyncUpdate() {};

    void Execute() {
      m_data = program.update(m_data);
    };
    void OnOK() {
      std::string string = m_data.dump();
      Callback().Call({Env().Null(), Napi::String::New(Env(), string)});
    };
};

// -- -- -- -- -- Buffer

class AsyncBuffer : public Napi::AsyncWorker {

  private:
    Program& program;

    nlohmann::json m_data;

    Napi::Uint8Array p_image;
    int m_width, m_height;

    cv::Mat m_image;

  public:
    AsyncBuffer(Napi::Function& callback, Program& program, nlohmann::json data, Napi::Uint8Array image)
      : AsyncWorker(callback), program(program), m_data(data), p_image(image) {

      m_width  = m_data["width"];
      m_height = m_data["height"];

      cv::Size size(m_width, m_height);

      m_image = cv::Mat::zeros(cv::Size(m_width, m_height), CV_8UC3);
    };
    virtual ~AsyncBuffer() {};

    void Execute() {
      m_data = program.buffer(m_data, m_image);
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

      std::string string = m_data.dump();
      Callback().Call({Env().Null(), Napi::String::New(Env(), string)});
    };

};

// -- -- -- -- -- Quit

class AsyncQuit : public Napi::AsyncWorker {

  private:

    Program& program;

  public:
    AsyncQuit(Napi::Function& callback, Program& program)
      : AsyncWorker(callback), program(program) {
    };
    virtual ~AsyncQuit() {};

    void Execute() {
      program.quit();
    };
    void OnOK() {
      std::string string = "program: quit done";
      Callback().Call({Env().Null(), Napi::String::New(Env(), string)});
    };
};

#endif
