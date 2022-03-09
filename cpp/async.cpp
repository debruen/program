
#include "async.h"

// -- -- -- -- -- Data

AsyncData::AsyncData(Napi::Function& callback, Program& program)
  : AsyncWorker(callback), program(program) {
};

void AsyncData::Execute() {
  m_data = program.data();
};

void AsyncData::OnOK() {
  std::string string = m_data.dump();
  Callback().Call({Env().Null(), Napi::String::New(Env(), string)});
};

// -- -- -- -- -- Update

AsyncUpdate::AsyncUpdate(Napi::Function& callback, Program& program, nlohmann::json data)
  : AsyncWorker(callback), program(program), m_data(data) {
};

void AsyncUpdate::Execute() {
  m_data = program.update(m_data);
};

void AsyncUpdate::OnOK() {
  std::string string = m_data.dump();
  Callback().Call({Env().Null(), Napi::String::New(Env(), string)});
};

// -- -- -- -- -- Buffer

AsyncBuffer::AsyncBuffer(Napi::Function& callback, Program& program, nlohmann::json data, Napi::Uint8Array image)
  : AsyncWorker(callback), program(program), m_data(data), p_image(image) {

  m_width  = m_data["width"];
  m_height = m_data["height"];

  cv::Size size(m_width, m_height);

  m_image = cv::Mat::zeros(cv::Size(m_width, m_height), CV_8UC3);
};

void AsyncBuffer::Execute() {
  m_data = program.buffer(m_data, m_image);
};

void AsyncBuffer::OnOK() {

  // writing result to image_buffer
  std::size_t c, z;
  uchar* image_ptr;

  for (std::size_t y = 0; y < m_height; y++) {

    image_ptr = m_image.ptr<uchar>(y);
    for (std::size_t x = 0; x < m_width; x++) {
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

// -- -- -- -- -- Quit

AsyncQuit::AsyncQuit(Napi::Function& callback, Program& program)
  : AsyncWorker(callback), program(program) {
};

void AsyncQuit::Execute() {
  program.quit();
};

void AsyncQuit::OnOK() {
  std::string string = "program: quit done";
  Callback().Call({Env().Null(), Napi::String::New(Env(), string)});
};
