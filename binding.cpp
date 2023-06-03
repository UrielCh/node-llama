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
    void SetRngSeed(const Napi::CallbackInfo& info);
    Napi::Value GetKvCacheTokenCount(const Napi::CallbackInfo& info);
    Napi::Value GetStateSize(const Napi::CallbackInfo& info);

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
        InstanceMethod("free", &LlamaContext::Free),
        InstanceMethod("setRngSeed", &LlamaContext::SetRngSeed),
        InstanceMethod("getKvCacheTokenCount", &LlamaContext::GetKvCacheTokenCount),
        InstanceMethod("getStateSize", &LlamaContext::GetStateSize)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("LlamaContext", func);
    return exports;
}

LlamaContext::LlamaContext(const Napi::CallbackInfo& info) : Napi::ObjectWrap<LlamaContext>(info)  {
    _llamaCtx = nullptr;
}

void LlamaContext::SetRngSeed(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    }

    if (!info[0].IsNumber()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    }

    int seed = info[0].As<Napi::Number>().Int32Value();
    llama_set_rng_seed(_llamaCtx, seed);
}

Napi::Value LlamaContext::GetKvCacheTokenCount(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int count = llama_get_kv_cache_token_count(_llamaCtx);
    return Napi::Number::New(env, count);
}

Napi::Value LlamaContext::GetStateSize(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    size_t size = llama_get_state_size(_llamaCtx);
    return Napi::Number::New(env, size);
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

Napi::Number ModelQuantizeWrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 4) {
        Napi::TypeError::New(env, "Wrong number of arguments")
            .ThrowAsJavaScriptException();
        return Napi::Number::New(env, 0);
    }

    int result = llama_model_quantize(
        info[0].As<Napi::String>().Utf8Value().c_str(),
        info[1].As<Napi::String>().Utf8Value().c_str(),
        static_cast<llama_ftype>(info[2].As<Napi::Number>().Int32Value()),
        info[3].As<Napi::Number>().Int32Value()
    );

    return Napi::Number::New(env, result);
}

Napi::Number ApplyLoRaFromFileWrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 4) {
        Napi::TypeError::New(env, "Wrong number of arguments")
            .ThrowAsJavaScriptException();
        return Napi::Number::New(env, 0);
    }

    LlamaContext* obj = Napi::ObjectWrap<LlamaContext>::Unwrap(info[0].As<Napi::Object>());
    int result = llama_apply_lora_from_file(
        obj->GetLlamaCtx(),
        info[1].As<Napi::String>().Utf8Value().c_str(),
        info[2].As<Napi::String>().Utf8Value().c_str(),
        info[3].As<Napi::Number>().Int32Value()
    );

    return Napi::Number::New(env, result);
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

Napi::Boolean MMapSupportedWrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    bool result = llama_mmap_supported();
    return Napi::Boolean::New(env, result);
}

Napi::Boolean MLockSupportedWrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    bool result = llama_mlock_supported();
    return Napi::Boolean::New(env, result);
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    LlamaContext::Init(env, exports);
    exports.Set(Napi::String::New(env, "initBackend"), Napi::Function::New(env, InitBackendWrapper));
    exports.Set(Napi::String::New(env, "printTimings"), Napi::Function::New(env, PrintTimingsWrapper));
    exports.Set("modelQuantize", Napi::Function::New(env, ModelQuantizeWrapper));
    exports.Set("applyLoRaFromFile", Napi::Function::New(env, ApplyLoRaFromFileWrapper));
    exports.Set(Napi::String::New(env, "free"), Napi::Function::New(env, FreeWrapper));
    exports.Set(Napi::String::New(env, "mmapSupported"), Napi::Function::New(env, MMapSupportedWrapper));
    exports.Set(Napi::String::New(env, "mlockSupported"), Napi::Function::New(env, MLockSupportedWrapper));

    return exports;
}

NODE_API_MODULE(llama, InitAll)
