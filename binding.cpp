#include <napi.h>
#include "llama/llama.h"

class LlamaContext : public Napi::ObjectWrap<LlamaContext> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    LlamaContext(const Napi::CallbackInfo& info);
    ~LlamaContext();

private:
    static Napi::FunctionReference constructor;
    struct llama_context* _llamaCtx;

    // Binding methods
    void Free(const Napi::CallbackInfo& info);
};

Napi::FunctionReference LlamaContext::constructor;

static Napi::Value InitBackendWrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    llama_init_backend();
    return env.Null();
}

Napi::Object LlamaContext::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "LlamaContext", {
        InstanceMethod("free", &LlamaContext::Free)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("LlamaContext", func);
    return exports;
}

LlamaContext::LlamaContext(const Napi::CallbackInfo& info) : Napi::ObjectWrap<LlamaContext>(info)  {
    _llamaCtx = nullptr;
}

LlamaContext::~LlamaContext() {
    if (_llamaCtx != nullptr) {
        llama_free(_llamaCtx);
    }
}

void LlamaContext::Free(const Napi::CallbackInfo& info) {
    if (_llamaCtx != nullptr) {
        llama_free(_llamaCtx);
        _llamaCtx = nullptr;
    }
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    LlamaContext::Init(env, exports);
    exports.Set(Napi::String::New(env, "initBackend"), Napi::Function::New(env, InitBackendWrapper));
    return exports;
}

NODE_API_MODULE(llama, InitAll)
