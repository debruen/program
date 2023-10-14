
#include "input.h"

Input::Input() {

  /// 0: type
  nlohmann::json type;
  type["label"]   = "type";
  type["select"]  = "select";  // select, range, files
  type["options"] = {"image", "audio"};
  type["default"] = type["options"][0];
  type["value"]   = type["default"];

  m_data.push_back(type);

  /// 1: frames
  nlohmann::json frames;
  frames["label"]   = "frames";
  frames["select"]  = "range";  // select, range, files
  frames["type"]    = "int";
  frames["display"] = "select";
  frames["min"]     = 1;
  frames["max"]     = 60;
  frames["default"] = frames["min"];
  frames["value"]   = frames["default"];

  m_data.push_back(frames);

  /// 2: time (audio length)
  nlohmann::json time;
  time["label"]   = "time";
  time["select"]  = "range";  // select, range, files
  time["type"]    = "time";
  time["display"] = "select";
  time["min"]     = 1000;  // 1 sec
  time["max"]     = 60000; // 1 min
  time["default"] = 3000;  // 3 sec
  time["value"]   = time["default"];

  m_data.push_back(time);

  /// 3: ratio
  nlohmann::json ratio;
  ratio["label"]   = "ratio";
  ratio["select"]  = "range";  // select, range, files
  ratio["type"]    = "float";
  ratio["display"]  = "select";
  ratio["min"]     = 0.5;         // 1 : 2
  ratio["max"]     = 2;           // 2 : 1
  ratio["default"] = 1.0/M_SQRT2; // DIN
  ratio["value"]   = ratio["default"];

  m_data.push_back(ratio);

  /// 4: direction
  nlohmann::json direction;
  direction["label"]   = "direction";
  direction["select"]  = "select";  // select, range, files
  direction["options"] = {"up", "right", "down", "left"};
  direction["default"] = direction["options"][0];
  direction["value"]   = direction["default"];

  m_data.push_back(direction);

  /// 5: stereo
  nlohmann::json stereo;
  stereo["label"]   = "stereo";
  stereo["select"]  = "select";  // select, range, files
  stereo["options"] = {"lr", "rl"};
  stereo["default"] = stereo["options"][0];
  stereo["value"]   = stereo["default"];

  m_data.push_back(stereo);

  /// 6: files
  nlohmann::json files = nlohmann::json::array();
  m_data.push_back(files);

} // constructor END

void Input::crop(cv::Mat& image, unsigned int width, unsigned int height) {

  if (static_cast<unsigned int>(image.cols) != width || static_cast<unsigned int>(image.rows) != height) {

    cv::Mat temp;

    double a = width / static_cast<double>(height);
    double b = image.cols / static_cast<double>(image.rows);
    if(a < b) { // portrait
      unsigned int w, x0, x1;

      w = round(image.cols * (static_cast<float>(height) / image.rows));

      cv::Size size(w, height);
      cv::resize(image, temp, size, 0, 0, cv::INTER_CUBIC);

      x0 = (temp.cols - width) / 2;
      x1 = x0 + width;

      cv::Rect roi(x0, 0, x1, temp.rows);
      image = temp(roi);

    } else { // landscape
      unsigned int h, y0, y1;

      h = round(image.rows * (static_cast<float>(width) / image.cols));

      cv::Size size(width, h);
      cv::resize(image, temp, size, 0, 0, cv::INTER_CUBIC);

      y0 = (temp.rows - height) / 2;
      y1 = y0 + height;

      cv::Rect roi(0, y0, temp.cols, y1);
      image = temp(roi);

    }
  }

} // crop END

void Input::flip(cv::Mat& image, bool back) {

  // direction up
  if (m_data[4]["value"] == "up") {
    // stereo rl
    if (m_data[5]["value"] == "rl")
      cv::flip(image, image, 1);
  }

  // direction right
  if (m_data[4]["value"] == "right") {
    if (back) { // flip back
      if (m_data[5]["value"] == "rl")
        cv::flip(image, image, 1);
      cv::rotate(image, image, cv::ROTATE_90_CLOCKWISE);
    } else {    // flip
      cv::rotate(image, image, cv::ROTATE_90_COUNTERCLOCKWISE);
      if (m_data[5]["value"] == "rl")
        cv::flip(image, image, 1);
    }
  }

  // direction down
  if (m_data[4]["value"] == "down") {
    cv::flip(image, image, 0);
    // stereo rl
    if (m_data[5]["value"] == "rl")
      cv::flip(image, image, 1);
  }

  // directionleft
  if (m_data[4]["value"] == "left") {
    if (back) { // flip back
      if (m_data[5]["value"] == "lr")
        cv::flip(image, image, 1);
      cv::rotate(image, image, cv::ROTATE_90_COUNTERCLOCKWISE);
    } else {    // flip
      cv::rotate(image, image, cv::ROTATE_90_CLOCKWISE);
      // stereo rl
      if (m_data[5]["value"] == "lr")
        cv::flip(image, image, 1);
    }
  }

} // flip END

nlohmann::json Input::data() {

  return m_data;
} // data() END

nlohmann::json Input::data(nlohmann::json data) {

  /// update 0: type
  if (data[0]["value"] != m_data[0]["value"])
    data[6] = nlohmann::json::array();

  /// update 6: files (images)
  if (data[0]["value"] == "image" && data[6].size() > 0) {

    int max;

    /// limit image files to max frames
    if (data[6].size() > m_data[1]["max"]) {
      max = m_data[1]["max"];
    } else {
      max = data[6].size();
    }

    nlohmann::json data_temp;
    data_temp = nlohmann::json::array();

    for (int i = 0; i < max; i++) {
      /// getting width and height for image file if not already set
      if (data[6][i]["width"] == 0) {
        cv::Mat image_file = cv::imread(data[6][i]["file"], cv::IMREAD_COLOR);
        data[6][i]["width"] = image_file.cols;
        data[6][i]["height"] = image_file.rows;
      }
      data_temp.push_back(data[6][i]);
    }
    data[6] = data_temp;

    /// set frames by amount of image files
    data[1]["value"] = data[6].size();

    /// set ratio by first image file
    double width, height, ratio;
    width = data[6][0]["width"];
    height = data[6][0]["height"];
    ratio = width / height;
    data[3]["value"] = ratio;

    /// deactivate frames select for files
    data[1]["display"] = "value";
    /// deactivate ratio select for files
    data[3]["display"] = "value";

  } else {

    /// activate frames select for no files
    data[1]["display"] = "select";
    /// activate ratio select for no files
    data[3]["display"] = "select";

  }

  /// fit ratio to min max settings
  if (data[3]["value"] > m_data[3]["max"])
    data[3]["value"] = m_data[3]["max"];

  if (data[3]["value"] < m_data[3]["min"])
    data[3]["value"] = m_data[3]["min"];

  /// update 6: files (audio)
  if (data[0]["value"] == "audio" && data[6].size() > 0) {

    int max;

    /// limit audio files to 1
    if (data[6].size() > 1) {
      max = 1;
    } else {
      max = data[6].size();
    }

    nlohmann::json data_temp = nlohmann::json::array();

    for (int i = 0; i < max; i++) {
      /// getting time for audio file if not already set
      if (data[6][i]["time"] == 0) {
        stk::FileRead audio_file(data[6][0]["file"]);
        unsigned int time = audio_file.fileSize() / audio_file.fileRate() * 1000;
        data[6][i]["time"] = time;
      }
      data_temp.push_back(data[6][i]);
    }
    data[6] = data_temp;

    /// setting time to audio file time
    data[2]["value"] = data[6][0]["time"];

    /// activate time select for no files
    data[2]["display"] = "value";
  } else {

    /// activate time select for no files
    data[2]["display"] = "select";

  }

  /// fit time to min max settings
  if (data[2]["value"] > m_data[2]["max"])
    data[2]["value"] = m_data[2]["max"];

  if(data[2]["value"] < m_data[2]["min"])
    data[2]["value"] = m_data[2]["min"];

  m_data = data;

  return m_data;
} // data(data) END

void Input::process(std::vector<cv::Mat>& images, stk::StkFrames& audio) {
  unsigned int width, height, frames;
  width = images[0].cols;
  height = images[0].rows;
  frames = audio.frames();

  // init images
  // loop over frames
  for (int i = 0; i < m_data[1]["value"]; i++) {

    if(m_data[0]["value"] == "image" && m_data[6].size() > 0) {
      cv::Mat temp_image = cv::imread(m_data[6][i]["file"], cv::IMREAD_COLOR);
      crop(temp_image, width, height);
      images[i] = temp_image;
    } else {
      images[i] = 0;
    }

    flip(images[i]);
  } // loop over frames end

  if(m_data[0]["value"] == "audio" && m_data[6].size() > 0) {
    stk::FileRead audio_file(m_data[6][0]["file"]);
    stk::StkFrames audio_temp;
    audio_temp.resize(audio_file.fileSize(), audio_file.channels());
    audio_file.read(audio_temp);

    unsigned int max, min, sample_rate, max_frames, min_frames, file_frames;
    max = m_data[2]["max"];
    min = m_data[2]["min"];
    sample_rate = 44100.0;
    file_frames = audio_temp.frames();
    max_frames = max / 1000 * sample_rate;
    min_frames = min / 1000 * sample_rate;

    if (file_frames > max_frames) {
      file_frames = max_frames;
    }

    if (file_frames < min_frames) {
      file_frames = min_frames;
    }

    audio_temp.resize(file_frames, 2);
    audio = audio_temp;
  } else {
    stk::StkFrames empL(frames, 1), empR(frames, 1);
    // empL.setChannel(0, audio, 0);
    // empR.setChannel(0, audio, 1);
    for (unsigned int i = 0; i < empL.frames(); i++) {
      empL[i] = 0;
      empR[i] = 0;
    }
    audio.setChannel(0, empL, 0);
    audio.setChannel(1, empR, 0);
  }


} // process END

void Input::flip_back(std::vector<cv::Mat>& images) {

  for (unsigned int i = 0; i < images.size(); i++) {
    flip(images[i], true);
  } // loop over frames end

} // flip_back END
