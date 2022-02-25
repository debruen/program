
#include "async.h"

// -- -- -- -- -- Main

// AsyncMain::AsyncMain(Napi::Function& callback, Program& program)
//   : AsyncWorker(callback), program(program) {
// };
//
// void AsyncMain::Execute() {
//   program.main();
// };
//
// void AsyncMain::OnOK() {
//   std::string msg = "main";
//   Callback().Call({Env().Null(), Napi::String::New(Env(), msg)});
// };

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

// -- -- -- -- -- Read

AsyncRead::AsyncRead(Napi::Function& callback, Program& program, Napi::Uint8Array image, Napi::Float32Array left, Napi::Float32Array right, std::size_t frame_index)
  : AsyncWorker(callback), program(program), p_image(image), p_left(left), p_right(right), m_index(frame_index) {

  nlohmann::json data = program.data();

  m_width  = data::get_width(data["settings"], "preview");
  m_height = data::get_height(data["settings"], "preview");
  m_time   = data::get_int(data["settings"], "frame time") / 1000.0 * 44100.0;

  cv::Size size(m_width, m_height);

  m_image = cv::Mat::zeros(cv::Size(m_width, m_height), CV_8UC3);
  m_audio = cv::Mat::zeros(cv::Size(2, m_time), CV_32F);
};

void AsyncRead::Execute() {
  program.read(m_image, m_audio, m_index);
};

void AsyncRead::OnOK() {

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

  float* audio_ptr;

  for (std::size_t i = 0; i < m_time; i++) {

    audio_ptr = m_audio.ptr<float>(i);

    p_left[i]  = audio_ptr[0];
    p_right[i] = audio_ptr[1];

  }

  std::string string = "done";

  Callback().Call({Env().Null(), Napi::String::New(Env(), string)});
};

// -- -- -- -- -- Buffer

AsyncBuffer::AsyncBuffer(Napi::Function& callback, Program& program, nlohmann::json data, Napi::Uint8Array image)
  : AsyncWorker(callback), program(program), m_data(data), p_image(image) {

  m_width  = m_data["width"];
  m_height = m_data["height"];;

  cv::Size size(m_width, m_height);

  m_image = cv::Mat::zeros(cv::Size(m_width, m_height), CV_8UC3);
};

void AsyncBuffer::Execute() {
  program.buffer(m_data, m_image);
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

  std::string string = "done";

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


// AsyncPreview::AsyncPreview(Napi::Function& callback, Program& program, Napi::Uint8Array images, Napi::Float32Array left, Napi::Float32Array right)
//   : AsyncWorker(callback), program(program), p_images(images), p_left(left), p_right(right) {
//
//   nlohmann::json data = program.data();
//
//   m_width  = data::get_width(data["settings"], "preview");
//   m_height = data::get_height(data["settings"], "preview");
//   m_frames = data::get_int(data["settings"], "frames");
//   m_time   = data::get_int(data["settings"], "time") / 1000.0 * 44100.0;
//
//   cv::Size size(m_width, m_height);
//
//   for (unsigned int i = 0; i < m_frames; i++) {
//     cv::Mat image = cv::Mat(size, CV_8UC3);
//     image = 0;
//     m_images.push_back(image);
//   }
//
//   m_audio.resize(m_time, 2);
// };
//
// void AsyncPreview::Execute() {
//
//   program.preview(m_images, m_audio);
//
// };
//
// void AsyncPreview::OnOK() {
//
//   // writing result to image_buffer
//   unsigned int p, c, z;
//   uchar* ptr;
//
//   for (unsigned int i = 0; i < m_frames; i++) {
//     p = m_width * m_height * 4 * i;
//
//     for (unsigned int y = 0; y < m_height; y++) {
//       ptr = m_images[i].ptr<uchar>(y);
//
//       for (unsigned int x = 0; x < m_width; x++) {
//         z = (y * m_width + x) * 4 + p;
//         c = x * 3;
//
//         p_images[z]   = ptr[c+2]; // red
//         p_images[z+1] = ptr[c+1]; // green
//         p_images[z+2] = ptr[c];   // blue
//         p_images[z+3] = 255;      // alpha channel
//
//       }
//     }
//   }
//
//   // writing result to audio Buffer
//   stk::StkFrames framesL(m_time, 1), framesR(m_time, 1);
//
//   framesL.setChannel(0, m_audio, 0);
//   framesR.setChannel(0, m_audio, 1);
//
//   for(unsigned int i = 0; i < m_time; i++) {
//
//     p_left[i] = framesL[i];
//     p_right[i] = framesR[i];
//   }
//
//   std::string string = "done";
//
//   Callback().Call({Env().Null(), Napi::String::New(Env(), string)});
// };

// // AsyncSave
// AsyncSave::AsyncSave(Napi::Function& callback, Program& program)
//   : AsyncWorker(callback), program(program) {
//
// };
//
// void AsyncSave::Execute() {
//   program.save();
// };
//
// void AsyncSave::OnOK() {
//
//   std::string string = "done";
//
//   Callback().Call({Env().Null(), Napi::String::New(Env(), string)});
// };
