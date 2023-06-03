#ifndef LLAMA_CONTEXT_PARAMS_H
#define LLAMA_CONTEXT_PARAMS_H

#include <napi.h>
#include "../llama/llama.h"

class LlamaContextParams : public Napi::ObjectWrap<LlamaContextParams> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    LlamaContextParams(const Napi::CallbackInfo& info);
    llama_context_params* GetInternalInstance();

private:
    static Napi::FunctionReference constructor;
    Napi::Value GetNCtx(const Napi::CallbackInfo& info);
    void SetNCtx(const Napi::CallbackInfo& info, const Napi::Value& value);

    Napi::Value GetNGpuLayers(const Napi::CallbackInfo& info);
    void SetNGpuLayers(const Napi::CallbackInfo& info, const Napi::Value& value);

    Napi::Value GetSeed(const Napi::CallbackInfo& info);
    void SetSeed(const Napi::CallbackInfo& info, const Napi::Value& value);

    Napi::Value GetF16Kv(const Napi::CallbackInfo& info);
    void SetF16Kv(const Napi::CallbackInfo& info, const Napi::Value& value);

    Napi::Value GetLogitsAll(const Napi::CallbackInfo& info);
    void SetLogitsAll(const Napi::CallbackInfo& info, const Napi::Value& value);

    Napi::Value GetVocabOnly(const Napi::CallbackInfo& info);
    void SetVocabOnly(const Napi::CallbackInfo& info, const Napi::Value& value);

    Napi::Value GetUseMmap(const Napi::CallbackInfo& info);
    void SetUseMmap(const Napi::CallbackInfo& info, const Napi::Value& value);

    Napi::Value GetUseMlock(const Napi::CallbackInfo& info);
    void SetUseMlock(const Napi::CallbackInfo& info, const Napi::Value& value);

    Napi::Value GetEmbedding(const Napi::CallbackInfo& info);
    void SetEmbedding(const Napi::CallbackInfo& info, const Napi::Value& value);

    llama_context_params* _llama_context_params;
};

#endif