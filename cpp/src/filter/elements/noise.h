
#ifndef noise_h
#define noise_h

#include "nlohmann/json.hpp"

class Noise {

  private:

    nlohmann::json m_noise_data;

    nlohmann::json get_noise(std::string type);

  public:
    Noise(std::string type);

    virtual nlohmann::json data(std::string type);

    virtual nlohmann::json data(nlohmann::json data, std::string type);

    virtual void process(std::vector<cv::Mat>& images);

    virtual void process(stk::StkFrames& audio);

};

#endif // noise_h END
