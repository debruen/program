#ifndef nodeprogram_h
#define nodeprogram_h

#include <string>
#include <vector>
#include <napi.h>

#include "src/program.h"

class NodeProgram : public Napi::ObjectWrap<NodeProgram> {

  private:
    static Napi::FunctionReference constructor; // reference to store the class definition that needs to be exported to JS

    Napi::Value WrapData(const Napi::CallbackInfo& info);
    Napi::Value WrapProcess(const Napi::CallbackInfo& info);

    Program *program_; // internal instance of Ia used to perform actual operations.

    unsigned int m_display_width;
    unsigned int m_display_height;
    int m_frames;

  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports); // Init function for setting the export key to JS
    NodeProgram(const Napi::CallbackInfo& info); // Constructor to initialise

};

#endif /* nodeprogram_h */
