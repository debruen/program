
#ifndef sine_h
#define sine_h

#include <cstring>

// #include "../../functionality/data.h"
#include "../../functionality/math.h"

class Sine {

  private:
    const int& m_width,& m_height;
    const std::size_t& m_index;
    const std::string& m_shape;

    double m_proto_frequency, m_proto_phase, m_proto_tilt;
    std::string m_type;

    double m_frequency{0}, m_phase, m_tilt;

    void adjust_basics();

    double m_sine_width, m_sine_height, m_x_multi, m_y_multi, m_tilt_add_angle, m_shape_note{0};

    void adjust_tilt();

  public:
    Sine(int& width, int& height, std::size_t& index, const std::string& shape, const double frequency, double phase, const double tilt, std::string type);

    void set_frequency(double frequency);
    void set_phase(double phase);
    void set_tilt(double tilt);

    double point(int& y, int& x);

    double point_gradient(unsigned int y, unsigned int x, unsigned int width, unsigned int height, double frequency, double phase, double tilt);

};

#endif // sine_h END
