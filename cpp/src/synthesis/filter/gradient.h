
#ifndef gradient_h
#define gradient_h

#include "mask.h"

#include "sine.h"

class Gradient : public Mask{

  private:
    nlohmann::json m_data;

    const double m_freq_min{0}, m_freq_max{1000}, m_phase_min{0}, m_phase_max{1}, m_frq_gamma{6};

    double m_norm_low{-1}, m_norm_high{1};

    std::string m_shape{"sine"}, m_filter{"none"};

    double m_frequency{0}, m_amplitude{1}, m_phase{0}, m_tilt{0};

    void set_area_frequency(double& frequency);

  public:
    Gradient();

    virtual nlohmann::json init();

    virtual nlohmann::json update(nlohmann::json data);

    virtual cv::Mat frame(cv::Mat& mask, std::size_t frame_index, std::string type);

    virtual void process(cv::Mat& mask, std::size_t frame_index, std::string type);
};

#endif // gradient_h END
