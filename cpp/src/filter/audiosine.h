
#ifndef audiosine_h
#define audiosine_h

#include <cstring>

#include "../data.h"
#include "../math.h"

class AudioSine {

  private:

    const std::size_t& m_width,& m_height,& m_frame_index;
    const std::string& m_shape;

    const double& m_frequency,& m_tilt;

    double m_phase, m_shape_note{0};

    void frame_phase();

  public:
    AudioSine(const std::size_t& width, const std::size_t& height, const std::size_t& frame_index, const double& frequency, double phase, const double& tilt, const std::string& shape);

    double point(std::size_t& y, std::size_t& x);

};

#endif // audiosine_h END
