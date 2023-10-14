
#include "output.h"

Output::Output() {

  m_base = new Hsl();
  m_data = m_base->data();
}

nlohmann::json Output::data() {

  return m_data;
}

nlohmann::json Output::data(nlohmann::json data) {

  /// clear transform class if value is changed
  if(data[0]["value"] != m_data[0]["value"]) {
    if (data[0]["value"] == "hsl")
      m_base = new Hsl();

    if (data[0]["value"] == "rgb")
      m_base = new Hsl();

    data = m_base->data();
  } else {
    data = m_base->data(data);

  }

  m_data = data;

  return m_data;
}

void Output::process(std::vector<cv::Mat>& images, stk::StkFrames& audio) {

  m_base->process(images, audio);

}

void Output::process(stk::StkFrames& audio, std::vector<cv::Mat>& images) {

  m_base->process(audio, images);

}
