#include "LlamaContext.hh"

Napi::FunctionReference LlamaContext::constructor;

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
