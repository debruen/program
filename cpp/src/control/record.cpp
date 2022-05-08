
#include "record.h"

Record::Record(std::vector<frame>& buffer, std::mutex& buffer_mutex, info& info)
    : m_buffer(buffer), m_buffer_mutex(buffer_mutex), m_info(info) {

  m_path = getenv("HOME");
  m_path = m_path + "/Desktop/output/";
  m_name = "output_";

}

bool Record::file_exist(const std::string& file) {
  struct stat buffer;
  return (stat(file.c_str(), &buffer) == 0);
}

std::string Record::file(std::string type) {

  std::string index, imageTest, audioTest, textTest, file;

  for(int i = 0; i < 10000; ++i) {
    if(i < 10 - 1) {
      index = "000" + std::to_string(i + 1);
    } else if(i < 100 - 1) {
      index = "00" + std::to_string(i + 1);
    } else if(i < 1000 - 1) {
      index = "0" + std::to_string(i + 1);
    } else {
      index = std::to_string(i + 1);
    }

    imageTest = m_path + m_name + index + ".tiff";
    audioTest = m_path + m_name + index + ".aiff";
    textTest = m_path + m_name + index + ".txt";

    if(!file_exist(imageTest) && !file_exist(audioTest)) {
      break;
    }
  }

  if(type == "image") {
    file = imageTest;
  } else if(type == "audio") {
    file = audioTest;
  } else if(type == "text") {
    file = textTest;
  } else {
    file = m_path + m_name + index;
  }

  return file;
}

bool Record::frame_exists(std::size_t& frame_index) {
  bool check = false;

  m_buffer_mutex.lock();
  for (std::size_t i = 0; i < m_buffer.size(); i++) {
    if (m_buffer[i].index == frame_index) {
      check = true;
      break;
    }
  }
  m_buffer_mutex.unlock();

  return check;
}

frame Record::get_frame(std::size_t& frame_index) {

  frame frame;

  m_buffer_mutex.lock();
  for (std::size_t i = 0; i < m_buffer.size(); i++) {
    if (m_buffer[i].index == frame_index) {
      frame = m_buffer[i];
      break;
    }
  }
  m_buffer_mutex.unlock();

  return frame;
}

void Record::start() {

  m_frames = m_info.frames;
  while (!m_info.full) {
    /* code */
  }

  m_image = file("none");
  m_audio = file("audio");

  try {
    audio_out.openFile(m_audio, 2, stk::FileWrite::FILE_AIF, stk::Stk::STK_FLOAT32);
  } catch (stk::StkError &) {
    exit(1);
  }

}

void Record::save(std::size_t& frame_index) {

  while (!frame_exists(frame_index)) {}
  frame frame = get_frame(frame_index);

  save_image(frame.image, frame_index);
  save_audio(frame.audio);

}

void Record::save_image(cv::Mat& image, std::size_t& frame_index) {
  std::string file_path = m_image + " - " + std::to_string(frame_index + 1) + ".tiff";
  cv::imwrite(file_path, image);
}

void Record::save_audio(cv::Mat& audio) {

  stk::StkFrames left(audio.rows, 1), right(audio.rows, 1), output(audio.rows, 2);
  for (int i = 0; i < audio.rows; i++) {

    left[i]  = audio.ptr<double>(i)[0];
    right[i] = audio.ptr<double>(i)[1];

  }
  output.setChannel(0, left, 0);
  output.setChannel(1, right, 0);

  audio_out.tick(output);
}

bool Record::stop() {
  audio_out.closeFile();

  return false;
}

void Record::init(nlohmann::json& data) {
  data["record"] = false;
}

bool Record::record() {
  start();
  std::cout << "m_frames:" << m_frames << '\n';
  for(std::size_t i = 0; i < m_frames; i++) {
    save(i);
  }
  return stop();
}
