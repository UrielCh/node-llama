#include <napi.h>
#include "llama/llama.h"

class LlamaContext : public Napi::ObjectWrap<LlamaContext> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    LlamaContext(const Napi::CallbackInfo& info);
    ~LlamaContext();

    // Getter and setter for _llamaCtx
    struct llama_context* GetLlamaCtx() const { return _llamaCtx; }
    void SetLlamaCtx(struct llama_context* ctx) { _llamaCtx = ctx; }
    void Free(const Napi::CallbackInfo& info);

private:
    static Napi::FunctionReference constructor;
    struct llama_context* _llamaCtx;
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

static Napi::Value PrintTimingsWrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::Object obj = info[0].As<Napi::Object>();
    LlamaContext* context = Napi::ObjectWrap<LlamaContext>::Unwrap(obj);
    llama_print_timings(context->GetLlamaCtx());
    return env.Null();
}

static Napi::Value FreeWrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::Object obj = info[0].As<Napi::Object>();
    LlamaContext* context = Napi::ObjectWrap<LlamaContext>::Unwrap(obj);
    context->Free(info);
    return env.Null();
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    LlamaContext::Init(env, exports);
    exports.Set(Napi::String::New(env, "initBackend"), Napi::Function::New(env, InitBackendWrapper));
    exports.Set(Napi::String::New(env, "printTimings"), Napi::Function::New(env, PrintTimingsWrapper));
    exports.Set(Napi::String::New(env, "free"), Napi::Function::New(env, FreeWrapper));
    return exports;
}

NODE_API_MODULE(llama, InitAll)
