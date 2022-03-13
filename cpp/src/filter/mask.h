
#ifndef mask_h
#define mask_h

#include "../data.h"
#include "../math.h"

class Mask {

  private:

    nlohmann::json m_mask = nlohmann::json::array();

    std::string m_type{"gradient"};

  protected:

    Mask() {
      std::vector<std::string> type_options{"gradient"};
      m_mask.push_back(data::init_str("type", type_options, m_type));
    };

    nlohmann::json get_data() {
      return m_mask;
    };

  public:

    virtual nlohmann::json init() = 0;

    virtual nlohmann::json update(nlohmann::json data) = 0;

    virtual cv::Mat frame(cv::Mat& mask, std::size_t index) = 0;

    virtual void process(cv::Mat& mask, std::size_t index) = 0;

};

#endif // mask_h END
