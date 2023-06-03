#ifndef LLAMACONTEXT_HH
#define LLAMACONTEXT_HH

#include <napi.h>
#include "llama/llama.h"

class LlamaContext : public Napi::ObjectWrap<LlamaContext> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    LlamaContext();
    LlamaContext(const Napi::Env env, struct llama_context* ctx); // New constructor
    ~LlamaContext();

    // Getter and setter for _llamaCtx
    struct llama_context* GetLlamaCtx() const { return _llamaCtx; }
    void SetLlamaCtx(struct llama_context* ctx) { _llamaCtx = ctx; }
    void Free(const Napi::CallbackInfo& info);
    void SetRngSeed(const Napi::CallbackInfo& info);
    Napi::Value GetKvCacheTokenCount(const Napi::CallbackInfo& info);
    Napi::Value GetStateSize(const Napi::CallbackInfo& info);
    Napi::Value CopyStateData(const Napi::CallbackInfo& info);
    Napi::Value SetStateData(const Napi::CallbackInfo& info);
    Napi::Value LoadSessionFile(const Napi::CallbackInfo& info);
    Napi::Value SaveSessionFile(const Napi::CallbackInfo& info);
    Napi::Value Eval(const Napi::CallbackInfo& info);
    Napi::Value Tokenize(const Napi::CallbackInfo& info);
    Napi::Value NVocab(const Napi::CallbackInfo& info);
    Napi::Value NCtx(const Napi::CallbackInfo& info);
    Napi::Value NEmbd(const Napi::CallbackInfo& info);
    // Napi::Value GetLogits(const Napi::CallbackInfo& info);
    // Napi::Value GetEmbeddings(const Napi::CallbackInfo& info);
    Napi::Value TokenToStr(const Napi::CallbackInfo& info);
    Napi::Value TokenBOS(const Napi::CallbackInfo& info);
    Napi::Value TokenEOS(const Napi::CallbackInfo& info);
    Napi::Value TokenNL(const Napi::CallbackInfo& info);

private:
    static Napi::FunctionReference constructor;
    struct llama_context* _llamaCtx;
};

#endif