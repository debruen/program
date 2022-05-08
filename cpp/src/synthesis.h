
#ifndef synthesis_h
#define synthesis_h

#include <chrono>

#include <thread>
#include <mutex>

#include "synthesis/settings.h" // dimension
#include "synthesis/filter.h"   // admixture
#include "synthesis/output.h"   // transform

class Synthesis {

  private:

    std::vector<frame>& m_buffer;
    std::mutex& m_buffer_mutex;

    info& m_info;

    Settings m_settings;
    Filter   m_filter;
    Output   m_output;

    nlohmann::json m_data;
    std::mutex m_mutex;

    std::size_t m_frames{1}, m_start{0};
    int m_time{0};
    std::string m_type;

    bool m_full{false}, m_update{false};

    void create_frame(std::size_t index);
    void create_buffer();
    std::size_t last_index();
    void clear_buffer();

    bool m_quit{false};
    void thread();
    std::thread m_thread;

  public:
    Synthesis(std::vector<frame>& buffer, std::mutex& buffer_mutex, info& info);

    nlohmann::json init();

    nlohmann::json data(nlohmann::json data);

    bool quit();

};

#endif /* synthesis_h */
