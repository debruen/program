
#ifndef blend_h
#define blend_h

#include "gradient.h"

typedef unsigned char (*blendchar)(unsigned char&, unsigned char&, double&);
typedef double (*blendfloat)(double&, double&, double&);

class Blend {

  private:

    // std::string m_blend;

    blendchar m_blendc;
    blendfloat m_blendf;

    nlohmann::json m_array = nlohmann::json::array();

    nlohmann::json m_data;

    std::vector< Mask* > m_masks;

    blendchar get_blendc(nlohmann::json data, std::string label);
    blendfloat get_blendf(nlohmann::json data, std::string label);

    static double normal(double& a, double& b, double& o);
    static unsigned char normal(unsigned char& aChar, unsigned char& bChar, double& o);

    // mean
    static double arithmetic(double& a, double& b, double& o);
    static unsigned char arithmetic(unsigned char& aChar, unsigned char& bChar, double& o);

    static double geometric(double& a, double& b, double& o);
    static unsigned char geometric(unsigned char& aChar, unsigned char& bChar, double& o);

    static double harmonic(double& a, double& b, double& o);
    static unsigned char harmonic(unsigned char& aChar, unsigned char& bChar, double& o);

    static double darken(double& a, double& b, double& o);
    static unsigned char darken(unsigned char& aChar, unsigned char& bChar, double& o);

    static double multiply(double& a, double& b, double& o);
    static unsigned char multiply(unsigned char& aChar, unsigned char& bChar, double& o);

    static double colorburn(double& a, double& b, double& o);
    static unsigned char colorburn(unsigned char& aChar, unsigned char& bChar, double& o);

    static double linearburn(double& a, double& b, double& o);
    static unsigned char linearburn(unsigned char& aChar, unsigned char& bChar, double& o);

    static double lighten(double& a, double& b, double& o);
    static unsigned char lighten(unsigned char& aChar, unsigned char& bChar, double& o);

    static double screen(double& a, double& b, double& o);
    static unsigned char screen(unsigned char& aChar, unsigned char& bChar, double& o);

    static double colordodge(double& a, double& b, double& o);
    static unsigned char colordodge(unsigned char& aChar, unsigned char& bChar, double& o);

    static double lineardodge(double& a, double& b, double& o);
    static unsigned char lineardodge(unsigned char& aChar, unsigned char& bChar, double& o);

    static double overlay(double& a, double& b, double& o);
    static unsigned char overlay(unsigned char& aChar, unsigned char& bChar, double& o);

    static double softlight(double& a, double& b, double& o);
    static unsigned char softlight(unsigned char& aChar, unsigned char& bChar, double& o);

    static double hardlight(double& a, double& b, double& o);
    static unsigned char hardlight(unsigned char& aChar, unsigned char& bChar, double& o);

    static double vividlight(double& a, double& b, double& o);
    static unsigned char vividlight(unsigned char& aChar, unsigned char& bChar, double& o);

    static double linearlight(double& a, double& b, double& o);
    static unsigned char linearlight(unsigned char& aChar, unsigned char& bChar, double& o);

    static double pinlight(double& a, double& b, double& o);
    static unsigned char pinlight(unsigned char& aChar, unsigned char& bChar, double& o);

    static double hardmix(double& a, double& b, double& o);
    static unsigned char hardmix(unsigned char& aChar, unsigned char& bChar, double& o);

    static double difference(double& a, double& b, double& o);
    static unsigned char difference(unsigned char& aChar, unsigned char& bChar, double& o);

    static double exclusion(double& a, double& b, double& o);
    static unsigned char exclusion(unsigned char& aChar, unsigned char& bChar, double& o);

    void frame(cv::Mat& image, cv::Mat& film, std::size_t frame_index);

  public:
    Blend();

    nlohmann::json data();

    nlohmann::json update(nlohmann::json data);

    void image_frame(cv::Mat& image, cv::Mat& film, std::size_t frame_index);

    void audio_frame(cv::Mat& audio, cv::Mat& film, std::size_t frame_index);


    void process(std::vector<cv::Mat>& images, std::vector<cv::Mat>& fillings);

    void process(stk::StkFrames& audio, stk::StkFrames& filling);
};

#endif // blend_h END
