
#ifndef record_h
#define record_h

#include <thread>
#include <mutex>

#include <fstream>
#include <sys/stat.h>

#include "stk/FileWvOut.h"

#include "../functionality/data.h"

class Record {

  private:

    std::vector<frame>& m_buffer;
    std::mutex& m_buffer_mutex;

    info& m_info;
    std::mutex& m_info_mutex;

    std::string m_path, m_name, m_image, m_audio;

    std::size_t m_frames{1};

    stk::FileWvOut audio_out;

    bool file_exist(const std::string& file);
    std::string file(std::string type);

    bool frame_exists(std::size_t& frame_index);
    frame get_frame(std::size_t& frame_index);


    void start();

    void save(std::size_t& frame_index);

    void save_image(cv::Mat& image, std::size_t& frame_index);
    void save_audio(cv::Mat& audio);

    bool stop();


  public:
    Record(std::vector<frame>& buffer, std::mutex& buffer_mutex, info& info, std::mutex& info_mutex);

    void init(nlohmann::json& data);

    bool record();

};

#endif /* record_h */
