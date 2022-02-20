
#ifndef areasine_h
#define areasine_h

#include <cstring>

#include "../data.h"
#include "../math.h"

class AreaSine {

  private:

    const std::size_t& m_width,& m_height,& m_frame_index;
    const std::string& m_shape;

    double m_phase{0}, m_tilt{0}, m_shape_note{0};

    bool m_phase_set = false;

    void frame_phase(const double& frequency, const double& phase, const double& tilt);

  public:
    AreaSine(const std::size_t& width, const std::size_t& height, const std::size_t& frame_index, const std::string& shape);

    double point(const std::size_t& y, const std::size_t& x, const double& frequency, const double& phase, const double& tilt);

};

#endif // areasine_h END
