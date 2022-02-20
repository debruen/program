
#include "settings.h"

Settings::Settings() {

  /// type (image or audio)
  std::vector<std::string> type_options{"image", "audio"};
  m_data.push_back(data::init_str("type", type_options, type_options[0]));

  /// 3: ratio (2:1 - 1:2)
  m_data.push_back(data::data_float("ratio", 0.5, 2, 1.0/M_SQRT2));

  /// 4: area (area dimensions: A6, A5, A4, A3, A2)
  std::vector<std::string> area_options{"A6", "A5", "A4", "A3", "A2"};
  m_data.push_back(data::data_string("area", true, area_options, area_options[2]));

  /// 5: direction
  std::vector<std::string> direction_options{"up", "right", "down", "left"};
  m_data.push_back(data::data_string("direction", true, direction_options, direction_options[0]));

  /// 6: stereo
  std::vector<std::string> stereo_options{"lr", "rl"};
  m_data.push_back(data::data_string("stereo", true, stereo_options, stereo_options[0]));

  /// time per frame (audio length in milliseconds)
  m_data.push_back(data::init_time("frame time", 1000, 60000, 3000));


  // /// frames (image frames)
  // m_data.push_back(data::init_int("frames", 1, 60, 1));

  // /// time (audio length in milliseconds)
  // m_data.push_back(data::init_time("time", 1000, 60000, 3000));



  /// 7: display size
  m_data.push_back(data::data_size("preview"));

  /// 8: output size
  m_data.push_back(data::data_size("output"));

  // 9: path
  std::string save_path = getenv("HOME");
  save_path = save_path + "/Desktop/output/";
  m_data.push_back(data::data_path("path", save_path));

  // 10: file name
  std::string file_name = "output_";
  m_data.push_back(data::data_path("file name", file_name));

  update_data(m_data);

} // constructor() END!

// Settings::Settings(nlohmann::json data) : m_data(data) {
//
//   update_data(m_data);
//
// } // constructor(data) END!

void Settings::update_data(nlohmann::json& data) {

  std::string area = data::get_string(data, "area");
  double ratio = data::get_float(data, "ratio");
  data::compute_size(data, "output", area, ratio);

} // update_data(&data) END!

void Settings::flip_image(cv::Mat& image, bool back) {

  std::string direction = data::get_string(m_data, "direction");
  std::string stereo = data::get_string(m_data, "stereo");

  // direction up
  if (direction == "up") {
    // stereo rl
    if (stereo == "rl")
      cv::flip(image, image, 1);
  }

  // direction right
  if (direction == "right") {
    if (back) { // flip back
      if (stereo == "rl")
        cv::flip(image, image, 1);
      cv::rotate(image, image, cv::ROTATE_90_CLOCKWISE);
    } else {    // flip
      cv::rotate(image, image, cv::ROTATE_90_COUNTERCLOCKWISE);
      if (stereo == "rl")
        cv::flip(image, image, 1);
    }
  }

  // direction down
  if (direction == "down") {
    cv::flip(image, image, 0);
    // stereo rl
    if (stereo == "rl")
      cv::flip(image, image, 1);
  }

  // directionleft
  if (direction == "left") {
    if (back) { // flip back
      if (stereo == "lr")
        cv::flip(image, image, 1);
      cv::rotate(image, image, cv::ROTATE_90_COUNTERCLOCKWISE);
    } else {    // flip
      cv::rotate(image, image, cv::ROTATE_90_CLOCKWISE);
      // stereo rl
      if (stereo == "lr")
        cv::flip(image, image, 1);
    }
  }

} // flip_image(&image, back = false) END!

nlohmann::json Settings::data() {

  return m_data;
} // data() END

nlohmann::json Settings::update(nlohmann::json data) {

  m_data = data;

  update_data(m_data);

  return m_data;
} // update(data) END

cv::Mat Settings::image_frame(std::size_t frame_index) {

  std::size_t width, height;

  width = data::get_width(m_data, "output");
  height = data::get_height(m_data, "output");

  cv::Mat image = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);

  flip_image(image);

  return image;
}

cv::Mat Settings::audio_frame(std::size_t frame_index) {

  std::size_t height = data::get_int(m_data, "frame time") / 1000.0 * 44100.0;

  cv::Mat audio = cv::Mat::zeros(cv::Size(m_audio_channels, height), CV_32F);

  return audio;
}


void Settings::preview(std::vector<cv::Mat>& images, stk::StkFrames& audio) {

  std::size_t width, height, audio_frames, frames;
  width = images[0].cols;
  height = images[0].rows;
  audio_frames = audio.frames();

  frames = data::get_int(m_data, "frames");

  // init images
  for (std::size_t i = 0; i < frames; i++) {

    images[i] = 0;

    flip_image(images[i]);
  } // loop over frames end

  stk::StkFrames empL(audio_frames, 1), empR(audio_frames, 1);
  for (std::size_t i = 0; i < empL.frames(); i++) {
    empL[i] = 0;
    empR[i] = 0;
  }
  audio.setChannel(0, empL, 0);
  audio.setChannel(1, empR, 0);

} // process()

cv::Mat Settings::image() {

  std::size_t width, height;

  width = data::get_width(m_data, "output");
  height = data::get_height(m_data, "output");

  cv::Size size(width, height);

  cv::Mat image = cv::Mat(size, CV_8UC3);
  image = 0;

  flip_image(image);

  return image;
}

stk::StkFrames Settings::audio() {

  std::size_t frame_time = data::get_int(m_data, "frame time") / 1000.0 * 44100.0;

  stk::StkFrames audio(frame_time, 2);

  return audio;
}

void Settings::file(std::vector<cv::Mat>& images, stk::StkFrames& audio) {

  std::size_t width, height, audio_frames, image_frames;

  width = data::get_width(m_data, "output");
  height = data::get_height(m_data, "output");

  audio_frames   = data::get_int(m_data, "time") / 1000.0 * 44100.0;
  image_frames = data::get_int(m_data, "frames");

  cv::Size size(width, height);

  for (std::size_t i = 0; i < image_frames; i++) {

    cv::Mat img = cv::Mat(size, CV_8UC3);
    img = 0;
    flip_image(img);

    images.push_back(img);

  }

  audio.resize(audio_frames, 2, 0);

} // process()

void Settings::flip_back(cv::Mat& image) {

  flip_image(image, true);

} // flip_back()

void Settings::save(std::vector<cv::Mat>& images, stk::StkFrames& audio) {

  std::string path, file_name;
  path = data::get_string(m_data, "path");
  file_name = data::get_string(m_data, "file name");

  std::string image_file = blankFile(file_name, path);

  std::string audio_file = audioFile(file_name, path);

  for (std::size_t i = 0; i < images.size(); i++) {
    save_image(images[i], image_file, i);
  }

  save_audio(audio, audio_file);
}


std::string Settings::audioFile(std::string filename, std::string path) {
  std::string type = "audio";
  return getFreeFile(filename, path, type);
}

std::string Settings::blankFile(std::string filename, std::string path) {
  std::string type = "none";
  return getFreeFile(filename, path, type);
}

void Settings::save_image(cv::Mat& image, std::string file, unsigned int page) {
  std::string file_path = file + " - " + std::to_string(page + 1) + ".tiff";

  cv::imwrite( file_path, image );

} // save_image

void Settings::save_audio(stk::StkFrames& audio, std::string file) {

  stk::FileWvOut output;
  try {
      output.openFile(file, audio.channels(), stk::FileWrite::FILE_AIF, stk::Stk::STK_SINT16 );
  } catch (stk::StkError &) {
      exit(1);
  }
  output.tick(audio);
} // save

bool Settings::fileExists(const std::string& file) {
    struct stat buffer;
    return (stat(file.c_str(), &buffer) == 0);
}

std::string Settings::getFreeFile(std::string& filename, std::string& path, std::string type) {

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

        if(!fileExists(imageTest) && !fileExists(audioTest)) {
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
