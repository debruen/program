
#include "program.h"

Program::Program() {

  /// define the standard area in pixel for saved image files (A4 300 dpi)
  m_area = 2480 * 3508;

  /// initial buffer dimensions
  m_data["width"] = 1;
  m_data["height"] = 1;

  std::string home_dir = home();
  m_data["path"] = home_dir + "/Desktop/output/";

  m_data["name"] = "output_";

  m_data["input"] = m_input.data();

  /// init input type
  m_type = eval::data_str(m_data["input"], "type");

  m_data["filter"] = m_filtration.data();

  m_data["output"] = m_output.data();

} // constructor END

nlohmann::json Program::data() {

  return m_data;
} // data() END

nlohmann::json Program::data(nlohmann::json data) {

  data["input"] = m_input.data(data["input"]);

  /// init input type
  m_type = eval::data_str(data["input"], "type");

  data["filter"] = m_filtration.data(data["filter"], m_type);

  data["output"] = m_output.data(data["output"]);

  m_data = data;

  return m_data;
} // data(data) END

void Program::process(std::vector<cv::Mat>& images, stk::StkFrames& audio) {

  // input
  m_input.process(images, audio);

  // filtration
  m_filtration.process(images, audio, m_type);

  // output
  if (m_type == "image") {
    m_output.process(images, audio);
  } else {
    m_output.process(audio, images);
  }

  m_input.flip_back(images);

} // process(images, audio) END

void Program::process() {

  unsigned int width, height, area, frames;

  if (m_data["output"][6]["value"] == "A6") area = m_area / 4;
  if (m_data["output"][6]["value"] == "A5") area = m_area / 2;
  if (m_data["output"][6]["value"] == "A4") area = m_area;
  if (m_data["output"][6]["value"] == "A3") area = m_area * 2;
  if (m_data["output"][6]["value"] == "A2") area = m_area * 4;

  double ratio = m_data["input"][3]["value"];

  height = sqrt(area / ratio);
  width = height * ratio;

  std::vector<cv::Mat> images;

  cv::Size size(width, height);
  for (int i = 0; i < m_data["input"][1]["value"]; i++) {
    cv::Mat image = cv::Mat(size, CV_8UC3);
    images.push_back(image);
  }

  stk::StkFrames audio;

  // get audio buffer length
  unsigned int f = m_data["input"][2]["value"];
  frames = f / 1000 * 44100.0;
  audio.resize(frames, 2);

  m_input.process(images, audio);

  // filtration
  m_filtration.process(images, audio, m_type);

  if (m_data["input"][0]["value"] == "image") {
    m_output.process(images, audio);
  } else {
    m_output.process(audio, images);
  }

  m_input.flip_back(images);


  std::string image_file = blankFile(m_data["name"], m_data["path"]);

  std::string audio_file = audioFile(m_data["name"], m_data["path"]);

  for (unsigned int i = 0; i < m_data["input"][1]["value"]; i++) {
    save_image(images[i], image_file, i);
  }

  save(audio, audio_file);
} // process() END

std::string Program::audioFile(std::string filename, std::string path) {
    return getFreeFile(filename, path, AUDIO);
}

std::string Program::blankFile(std::string filename, std::string path) {
    return getFreeFile(filename, path, NONE);
}

void Program::save_image(cv::Mat& image, std::string file, unsigned int page) {
  std::string file_path = file + " - " + std::to_string(page + 1) + ".tiff";

  cv::imwrite( file_path, image );

} // save_image

void Program::save(stk::StkFrames& audio, std::string file) {

  stk::FileWvOut output;
  try {
      output.openFile( file, audio.channels(), stk::FileWrite::FILE_AIF, stk::Stk::STK_SINT16 );
  } catch (stk::StkError &) {
      exit(1);
  }
  output.tick(audio);
} // save

std::string Program::home() {
    return getenv("HOME");
}

bool Program::fileExists(const std::string& file) {
    struct stat buffer;
    return (stat(file.c_str(), &buffer) == 0);
}

std::string Program::getFreeFile(std::string& filename, std::string& path, TYPE t) {

    std::string index, imageTest, audioTest, textTest, file;

    // std::string path = home() + "/Desktop/out/";
    // path = home() + path;

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

        if(!fileExists(imageTest) && !fileExists(audioTest)) {
            break;
        }
    }

    if(t == IMAGE) {
        file = imageTest;
    } else if(t == AUDIO) {
        file = audioTest;
    } else if(t == TEXT) {
        file = textTest;
    } else {
        file = path + filename + index;
    }

    return file;
}
