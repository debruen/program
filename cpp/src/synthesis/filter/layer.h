
#ifndef layer_h
#define layer_h

#include "film.h"
#include "blend.h"

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


    void image(cv::Mat& image, std::size_t index) {
      cv::Mat film = m_film->image(image, index);
      m_blend->image(image, film, index);

      // m_blend->process(image, film);
    };

    void audio(cv::Mat& audio, std::size_t index) {
      cv::Mat film = m_film->audio(audio, index);
      m_blend->audio(audio, film, index);
    };

};

#endif // layer_h END
