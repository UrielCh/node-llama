#include <napi.h>
#include "../llama/llama.h"
// #include "LlamaContext.hh"
#include "LlamaContextParams.hh"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    // init lama on load
    llama_init_backend();
    // LlamaContext::Init(env, exports);
    LlamaContextParams::Init(env, exports);
    // exports.Set(Napi::String::New(env, "initBackend"), Napi::Function::New(env, InitBackendWrapper));
    // exports.Set(Napi::String::New(env, "printTimings"), Napi::Function::New(env, PrintTimingsWrapper));
    // exports.Set("modelQuantize", Napi::Function::New(env, ModelQuantizeWrapper));
    // exports.Set("applyLoRaFromFile", Napi::Function::New(env, ApplyLoRaFromFileWrapper));
    // exports.Set(Napi::String::New(env, "free"), Napi::Function::New(env, FreeWrapper));
    // exports.Set(Napi::String::New(env, "mmapSupported"), Napi::Function::New(env, MMapSupportedWrapper));
    // exports.Set(Napi::String::New(env, "mlockSupported"), Napi::Function::New(env, MLockSupportedWrapper));

    return exports;
}

NODE_API_MODULE(llama, InitAll)
