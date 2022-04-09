
#ifndef synthesis_h
#define synthesis_h

#include <chrono>

#include <thread>
#include <mutex>

#include "settings.h" // dimension
#include "filter.h"   // admixture
#include "output.h"   // transform

class Synthesis {

  private:

    std::vector<frame>& m_buffer;
    std::mutex& m_buffer_mutex;

    info& m_info;
    std::mutex& m_info_mutex;

    Settings m_settings;
    Filter   m_filter;
    Output   m_output;

    nlohmann::json m_data;
    std::mutex m_mutex;

    std::size_t m_frames{1};
    int m_time{0};
    std::string m_type;

    bool m_full{false}, m_update{false}, m_quit{false};

    void clear_buffer();
    void create_buffer();
    std::size_t last_buffer_index();
    void create_frame(std::size_t frame_index);

    void buffer();
    std::thread m_buffer_thread;

  public:
    Synthesis(std::vector<frame>& buffer, std::mutex& buffer_mutex, info& info, std::mutex& info_mutex);

    nlohmann::json init();

    nlohmann::json data(nlohmann::json data);

    bool quit();

};

#endif /* synthesis_h */
