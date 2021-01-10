#ifndef nodeclass_h
#define nodeclass_h

#include <string>
#include <vector>
#include <napi.h>

#include "opencv2/core.hpp"

#include "ia.h"

class NodeIA : public Napi::ObjectWrap<NodeIA> {

  private:
    static Napi::FunctionReference constructor; // reference to store the class definition that needs to be exported to JS

    // Napi::Value WrapSetData(const Napi::CallbackInfo& info);
    // Napi::Value WrapGetData(const Napi::CallbackInfo& info);

    // ------------------------------------------------------------------------- main settings

    Napi::Value WrapSetMode(const Napi::CallbackInfo& info);
    Napi::Value WrapGetMode(const Napi::CallbackInfo& info);

    // ------------------------------------------------------------------------- in settings

    Napi::Value WrapSetImageIn(const Napi::CallbackInfo& info);
    Napi::Value WrapGetImageIn(const Napi::CallbackInfo& info);

    Napi::Value WrapAddImageLayer(const Napi::CallbackInfo& info);
    Napi::Value WrapRmvImageLayer(const Napi::CallbackInfo& info);

    Napi::Value WrapAddImageFilter(const Napi::CallbackInfo& info);
    Napi::Value WrapRmvImageFilter(const Napi::CallbackInfo& info);

    Napi::Value WrapSetAudioIn(const Napi::CallbackInfo& info);
    Napi::Value WrapGetAudioIn(const Napi::CallbackInfo& info);

    Napi::Value WrapAddAudioTrack(const Napi::CallbackInfo& info);
    Napi::Value WrapRmvAudioTrack(const Napi::CallbackInfo& info);

    Napi::Value WrapAddAudioFilter(const Napi::CallbackInfo& info);
    Napi::Value WrapRmvAudioFilter(const Napi::CallbackInfo& info);

    // ------------------------------------------------------------------------- out settings

    Napi::Value WrapSetImageOut(const Napi::CallbackInfo& info);
    Napi::Value WrapGetImageOut(const Napi::CallbackInfo& info);

    Napi::Value WrapSetAudioOut(const Napi::CallbackInfo& info);
    Napi::Value WrapGetAudioOut(const Napi::CallbackInfo& info);

    Napi::Value WrapGetOutPages(const Napi::CallbackInfo& info);
    Napi::Value WrapGetOutTime(const Napi::CallbackInfo& info);

    // ------------------------------------------------------------------------- computation

    Napi::Value WrapPreview(const Napi::CallbackInfo& info);
    Napi::Value WrapRun(const Napi::CallbackInfo& info);

    Program *ia_; // internal instance of Ia used to perform actual operations.

  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports); // Init function for setting the export key to JS
    NodeIA(const Napi::CallbackInfo& info); // Constructor to initialise

};

#endif /* nodeclass_h */
