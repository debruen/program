#ifndef async_h
#define async_h

#include <napi.h>

#include <chrono>
#include <thread>

#include "src/program.h"

class AsyncInit : public Napi::AsyncWorker {

  private:

    Program& program;
    nlohmann::json m_data;

  public:
    AsyncInit(Napi::Function& callback, Program& program);
    virtual ~AsyncInit() {};

    void Execute();
    void OnOK();

};

class AsyncWork : public Napi::AsyncWorker {

  private:

    Program& program;
    std::string m_msg;

  public:
    AsyncWork(Napi::Function& callback, Program& program);
    virtual ~AsyncWork() {};

    void Execute();
    void OnOK();

};

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

class AsyncPreview : public Napi::AsyncWorker {

  private:
    Program& program;

    Napi::Uint8Array p_images;
    Napi::Float32Array p_left;
    Napi::Float32Array p_right;

    unsigned int m_width, m_height, m_frames, m_time;

    std::vector<cv::Mat> m_images;
    stk::StkFrames m_audio;

  public:
    AsyncPreview(Napi::Function& callback, Program& program, Napi::Uint8Array images, Napi::Float32Array left, Napi::Float32Array right);
    virtual ~AsyncPreview() {};

    void Execute();
    void OnOK();

};

class AsyncSave : public Napi::AsyncWorker {

  private:

    Program& program;

  public:
    AsyncSave(Napi::Function& callback, Program& program);
    virtual ~AsyncSave() {};

    void Execute();
    void OnOK();

};

#endif /* async_h */
