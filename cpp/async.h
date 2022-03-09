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

// -- -- -- -- -- Buffer

class AsyncBuffer : public Napi::AsyncWorker {

  private:
    Program& program;

    nlohmann::json m_data;

    Napi::Uint8Array p_image;
    std::size_t m_width, m_height;

    cv::Mat m_image;

  public:
    AsyncBuffer(Napi::Function& callback, Program& program, nlohmann::json data, Napi::Uint8Array images);
    virtual ~AsyncBuffer() {};

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

#endif
