
#ifndef settings_h
#define settings_h

#include "../functionality/data.h"

class Settings {

  private:
    nlohmann::json m_data = nlohmann::json::array();

    int m_width{0}, m_height{0}, m_channels{2}, m_time{0};

    void compute();

  public:
    Settings();

    nlohmann::json data();
    nlohmann::json update(nlohmann::json data);

    cv::Mat image();
    cv::Mat audio();

    void flip(cv::Mat& image, bool back = false);

};

#endif /* settings_h */
