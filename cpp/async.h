#ifndef async_h
#define async_h

#include <napi.h>

#include <chrono>
#include <thread>

#include "src/program.h"

// -- -- -- -- -- Main

// class AsyncMain : public Napi::AsyncWorker {
//
//   private:
//
//     Program& program;
//
//   public:
//     AsyncMain(Napi::Function& callback, Program& program);
//     virtual ~AsyncMain() {};
//
//     void Execute();
//     void OnOK();
// };

// -- -- -- -- -- Data

class AsyncData : public Napi::AsyncWorker {

  private:

    Program& program;
    nlohmann::json m_data;

  public:
    AsyncData(Napi::Function& callback, Program& program);
    virtual ~AsyncData() {};

    void Execute();
    void OnOK();
};

// -- -- -- -- -- Update

class AsyncUpdate : public Napi::AsyncWorker {

  private:

    Program& program;
    nlohmann::json m_data;

  public:
    AsyncUpdate(Napi::Function& callback, Program& program, nlohmann::json data);
    virtual ~AsyncUpdate() {};

    void Execute();
    void OnOK();
};

// -- -- -- -- -- Read

class AsyncRead : public Napi::AsyncWorker {

  private:
    Program& program;

    Napi::Uint8Array p_image;
    Napi::Float32Array p_left;
    Napi::Float32Array p_right;

    std::size_t m_width, m_height, m_time, m_index;

    cv::Mat m_image;
    cv::Mat m_audio;

  public:
    AsyncRead(Napi::Function& callback, Program& program, Napi::Uint8Array images, Napi::Float32Array left, Napi::Float32Array right, std::size_t frame_index);
    virtual ~AsyncRead() {};

    void Execute();
    void OnOK();

};

// -- -- -- -- -- Quit

class AsyncQuit : public Napi::AsyncWorker {

  private:

    Program& program;

  public:
    AsyncQuit(Napi::Function& callback, Program& program);
    virtual ~AsyncQuit() {};

    void Execute();
    void OnOK();
};


// class AsyncPreview : public Napi::AsyncWorker {
//
//   private:
//     Program& program;
//
//     Napi::Uint8Array p_images;
//     Napi::Float32Array p_left;
//     Napi::Float32Array p_right;
//
//     unsigned int m_width, m_height, m_frames, m_time;
//
//     std::vector<cv::Mat> m_images;
//     stk::StkFrames m_audio;
//
//   public:
//     AsyncPreview(Napi::Function& callback, Program& program, Napi::Uint8Array images, Napi::Float32Array left, Napi::Float32Array right);
//     virtual ~AsyncPreview() {};
//
//     void Execute();
//     void OnOK();
//
// };

// class AsyncSave : public Napi::AsyncWorker {
//
//   private:
//
//     Program& program;
//
//   public:
//     AsyncSave(Napi::Function& callback, Program& program);
//     virtual ~AsyncSave() {};
//
//     void Execute();
//     void OnOK();
//
// };

#endif
