
#ifndef layer_h
#define layer_h

#include "blend.h"
#include "film.h"

class Layer {

  private:

    // std::string m_type;

    nlohmann::json m_data;

    Blend* m_blend;
    Film* m_film;

  public:

    Layer() {

      m_data = nlohmann::json::array();

      m_blend = new Blend();
      m_film = new Film();
      m_data.push_back(m_film->data());
      m_data.push_back(m_blend->data());

    };

    nlohmann::json init() {
      return m_data;
    };

    nlohmann::json update(nlohmann::json data) {

      nlohmann::json new_data = nlohmann::json::array();

      new_data.push_back(m_film->update(data[0]));
      new_data.push_back(m_blend->update(data[1]));

      m_data = new_data;

      return m_data;
    };


    void image_frame(cv::Mat& image, std::size_t frame_index, std::string type) {

      cv::Mat film = m_film->image_frame(image, frame_index, type);
      m_blend->image_frame(image, film, frame_index, type);

      // m_blend->process(image, film);
    };

    void audio_frame(cv::Mat& audio, std::size_t frame_index, std::string type) {

      cv::Mat film = m_film->audio_frame(audio, frame_index, type);
      m_blend->audio_frame(audio, film, frame_index, type);
    };

};

#endif // layer_h END
