
#ifndef mask_h
#define mask_h

#include "../data.h"
#include "../math.h"

class Mask {

  private:

    nlohmann::json m_mask = nlohmann::json::array();

    std::string m_type{"gradient"};

  protected:

    Mask();

    nlohmann::json get_data();

  public:

    virtual nlohmann::json init() = 0;

    virtual nlohmann::json update(nlohmann::json data) = 0;

    virtual cv::Mat frame(cv::Mat& mask, std::size_t index) = 0;

    virtual void process(cv::Mat& mask, std::size_t index) = 0;
};

#endif // mask_h END
