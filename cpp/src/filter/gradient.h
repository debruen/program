
#ifndef gradient_h
#define gradient_h

#include "mask.h"

class Gradient : public Mask{

  private:

    nlohmann::json m_data;

    const double m_freq_min{0}, m_freq_max{1000}, m_frq_gamma{6};

    std::string m_shape{"sine"}, m_filter{"none"};

    double m_frequency{0}, m_amplitude{1}, m_phase{0}, m_tilt{0};

    std::size_t m_width{0}, m_height{0};

    // std::vector<bool> m_pattern;
    // std::vector<std::size_t> m_counter;

    double frame_phase(std::size_t index);

    double discrete(std::size_t& y, std::size_t& x, double& frequency, double& phase, double& tilt);

  public:
    Gradient();

    virtual nlohmann::json init();

    virtual nlohmann::json update(nlohmann::json data);

    virtual cv::Mat frame(cv::Mat& mask, std::size_t index);

    virtual void process(cv::Mat& mask, std::size_t index);
};

#endif // gradient_h END
