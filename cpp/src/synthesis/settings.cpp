
#include "settings.h"

Settings::Settings() {
  /// type (image or audio)
  std::vector<std::string> type_options{"image", "audio"};
  m_data.push_back(data::init_str("type", type_options, type_options[0]));

  /// ratio (2:1 - 1:2)
  m_data.push_back(data::init_float("ratio", 0.5, 2, 1.0/M_SQRT2));

  /// area (area compute: A6, A5, A4, A3, A2)
  std::vector<std::string> area_options{"A6", "A5", "A4", "A3", "A2"};
  m_data.push_back(data::init_str("area", area_options, area_options[2]));

  /// direction
  std::vector<std::string> direction_options{"up", "right", "down", "left"};
  m_data.push_back(data::init_str("direction", direction_options, direction_options[0]));

  /// stereo
  std::vector<std::string> stereo_options{"lr", "rl"};
  m_data.push_back(data::init_str("stereo", stereo_options, stereo_options[0]));

  /// frame time (audio length in milliseconds)
  m_data.push_back(data::init_time("frame time", 1000, 60000, 3000));

  /// frames
  m_data.push_back(data::init_int("frames", 1, 100, 1));

  /// frames
  m_data.push_back(data::init_int("start", 0, 100, 0));

  compute();
}

void Settings::compute() {

  std::string area = data::get_str(m_data, "area");
  double ratio = data::get_float(m_data, "ratio");

  /// base area = DIN A4 @ 300 dpi
  int base_area{2480 * 3508};

  if (area == "A6") base_area = base_area / 4;
  if (area == "A5") base_area = base_area / 2;
  // if (area == "A4") base_area = base_area;
  if (area == "A3") base_area = base_area * 2;
  if (area == "A2") base_area = base_area * 4;

  m_height = round(sqrt(base_area / ratio));
  m_width = round(m_height * ratio);

  m_time = data::get_int(m_data, "frame time") / 1000.0 * 44100.0;
}

nlohmann::json Settings::data() {

  return m_data;
} // data() END

nlohmann::json Settings::update(nlohmann::json data) {

  m_data = data;

  compute();

  return m_data;
} // update(data) END

cv::Mat Settings::image() {

  cv::Mat image = cv::Mat::zeros(cv::Size(m_width, m_height), CV_8UC3);

  flip(image);

  return image;
}

cv::Mat Settings::audio() {

  return cv::Mat::zeros(cv::Size(m_channels, m_time), CV_64F);
}

void Settings::flip(cv::Mat& image, bool back) {

  std::string direction = data::get_str(m_data, "direction");
  std::string stereo = data::get_str(m_data, "stereo");

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

}
