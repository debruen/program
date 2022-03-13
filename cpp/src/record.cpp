
#include "record.h"

Record::Record(std::vector<frame>& buffer)
  : m_buffer(buffer) {

  m_path = getenv("HOME");
  m_path = m_path + "/Desktop/output/";

  m_name = "output_";

}

bool Record::exists(const std::string& file) {
  struct stat buffer;
  return (stat(file.c_str(), &buffer) == 0);
}

std::string Record::file(std::string& filename, std::string& path, std::string type) {

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

    imageTest = path + filename + index + ".tiff";
    audioTest = path + filename + index + ".aiff";
    textTest = path + filename + index + ".txt";

    if(!exists(imageTest) && !exists(audioTest)) {
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
    file = path + filename + index;
  }

  return file;
}

bool Record::start() {

  m_image = file(m_name, m_path, "none");
  m_audio = file(m_name, m_path, "audio");

  try {
    audio_out.openFile(m_audio, 2, stk::FileWrite::FILE_AIF, stk::Stk::STK_SINT16);
  } catch (stk::StkError &) {
    exit(1);
  }
  return true;
}

bool Record::stop() {
  audio_out.closeFile();

  return false;
}

void Record::save(std::size_t frame_index) {
  if (m_prev != frame_index) {

    cv::Mat image, audio;

    for (std::size_t i = 0; i < m_buffer.size(); i++) {
      if (m_buffer[i].index == frame_index) {
        image = m_buffer[i].image;
        audio = m_buffer[i].audio;
        break;
      }
    }

    save_image(image, frame_index);

    save_audio(audio);

    m_prev = frame_index;
    std::cout << "save recording" << '\n';

  }
}

void Record::save_image(cv::Mat& image, std::size_t frame_index) {

  std::string file_path = m_image + " - " + std::to_string(frame_index + 1) + ".tiff";

  cv::imwrite(file_path, image);

} // save_image

void Record::save_audio(cv::Mat& audio) {

  stk::StkFrames left(audio.rows, 1), right(audio.rows, 1), output(audio.rows, 2);
  std::cout << "A" << '\n';
  for (int i = 0; i < audio.rows; i++) {

    double* ptr = audio.ptr<double>(i);

    left[i]  = ptr[0];
    right[i] = ptr[1];

  }
  std::cout << "B" << '\n';
  output.setChannel(0, left, 0);
  output.setChannel(1, right, 0);

  audio_out.tick(output);
}
