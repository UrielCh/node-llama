#ifndef LLAMACONTEXT_HH
#define LLAMACONTEXT_HH

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

#endif