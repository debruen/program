
#ifndef spectrum_h
#define spectrum_h

#include <cstring>

#include "coat.h"

#include "areasine.h"

class Spectrum : public Coat {

  private:

    nlohmann::json m_data;

    const double m_color_min{400}, m_color_max{700}, m_audio_min{16}, m_audio_max{21000}, m_frq_gamma{6};

    std::string m_shape{"sine"};

    double m_frequency{0.5}, m_amplitude{0.5}, m_phase{0}, m_tilt{0};

    std::vector< std::vector<unsigned char> > m_spectral_rgb;

    void spectral_rgb(double& r, double& g, double& b, double l);
    std::vector< std::vector<unsigned char> > rgb_spectrum();

    void set_audio_frequency(const std::size_t& height, double& frequency);

  public:
    Spectrum();

    virtual nlohmann::json data();

    virtual nlohmann::json update(nlohmann::json data);

    virtual cv::Mat image_frame(cv::Mat& image, std::size_t frame_index);

    virtual cv::Mat audio_frame(cv::Mat& audio, std::size_t frame_index);

};

#endif // spectrum_h END
