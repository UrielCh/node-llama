#include <napi.h>
#include "llama/llama.h" // include the header file of the C++ code you want to bind

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    // add your binding code here
    return exports;
}

NODE_API_MODULE(llama, InitAll)
