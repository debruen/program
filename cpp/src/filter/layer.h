
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


    void image_frame(cv::Mat& image, std::size_t frame_index);

    void audio_frame(cv::Mat& audio, std::size_t frame_index);

};

#endif // layer_h END
