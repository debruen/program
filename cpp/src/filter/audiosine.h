
#ifndef audiosine_h
#define audiosine_h

#include <cstring>

// #include "../data.h"
#include "../math.h"

class AudioSine {

  private:

    const int& m_width,& m_height;
    const std::size_t& m_index;
    const double m_frequency, m_tilt, m_point_tilt;
    const std::string& m_shape;

    double m_phase, m_shape_note{0};

    void frame_phase();

  public:
    AudioSine(int& width, int& height, std::size_t& index, const double& frequency, const double& tilt, const std::string& shape, double phase);

    double point(int& y, int& x);

};

#endif // audiosine_h END
