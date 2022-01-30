
#ifndef settings_h
#define settings_h

#include <fstream>
#include <sys/stat.h>

#include "stk/FileWvOut.h"

#include "data.h"

class Settings {

  private:

    nlohmann::json m_data = nlohmann::json::array();

    void update_data(nlohmann::json& data);

    void flip_image(cv::Mat& image, bool back = false);

    bool fileExists(const std::string& file);
    std::string getFreeFile(std::string& filename, std::string& path, std::string type);
    std::string audioFile(std::string filename, std::string path);
    std::string blankFile(std::string filename, std::string path);

    void save_image(cv::Mat& image, std::string file, unsigned int frame);
    void save_audio(stk::StkFrames& audio, std::string file);

  public:
    Settings();
    // Settings(nlohmann::json data);

    nlohmann::json data();

    nlohmann::json update(nlohmann::json data);

    cv::Mat image_frame(std::size_t frame);

    cv::Mat audio_frame(std::size_t frame);



    cv::Mat image();

    stk::StkFrames audio();

    void preview(std::vector<cv::Mat>& images, stk::StkFrames& audio);

    void file(std::vector<cv::Mat>& images, stk::StkFrames& audio);

    void flip_back(std::vector<cv::Mat>& images);

    void save(std::vector<cv::Mat>& images, stk::StkFrames& audio);

};

#endif /* settings_h */
