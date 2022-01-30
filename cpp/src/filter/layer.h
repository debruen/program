
#ifndef layer_h
#define layer_h

#include "blend.h"
#include "film.h"

class Layer {

  private:

    std::string m_type;

    nlohmann::json m_data;

    Blend* m_blend;
    Film* m_film;

  public:

    Layer();

    nlohmann::json init(std::string type);

    nlohmann::json update(nlohmann::json data, std::string type);


    void frame(cv::Mat& image, std::size_t frame);

    void frame(stk::StkFrames& audio, std::size_t frame);



    void process(std::vector<cv::Mat>& images, stk::StkFrames& audio);

};

#endif // layer_h END
