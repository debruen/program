
#ifndef film_h
#define film_h

// fillings
#include "spectrum.h"

class Film {

  private:

    std::string m_coat_type;

    nlohmann::json m_data;

    Coat* m_coat;

  public:

    Film() {

      m_coat_type = "spectrum";

      m_coat = new Spectrum();
      m_data = m_coat->data();
    };

    nlohmann::json data() {
      return m_data;
    };

    nlohmann::json update(nlohmann::json data) {

      std::string type = data::get_str(data, "type");

      if (type != m_coat_type) {

        if (type == "noise")
          m_coat = new Spectrum();
        else
          m_coat = new Spectrum();

        m_data = m_coat->data();

      } else {
        m_data = m_coat->update(data);
      }

      m_coat_type = type;


      return m_data;
    };

    cv::Mat image_frame(cv::Mat& image, std::size_t frame_index, std::string type) {

      return m_coat->image_frame(image, frame_index, type);
    };

    cv::Mat audio_frame(cv::Mat& audio, std::size_t frame_index, std::string type) {

      return m_coat->audio_frame(audio, frame_index, type);
    };

};

#endif // film_h END
