#include "LlamaContextParams.hh"

Napi::Object LlamaContextParams::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "LlamaContextParams", {
        InstanceAccessor("n_ctx", &LlamaContextParams::GetNCtx, &LlamaContextParams::SetNCtx),
        InstanceAccessor("n_gpu_layers", &LlamaContextParams::GetNGpuLayers, &LlamaContextParams::SetNGpuLayers),
        InstanceAccessor("seed", &LlamaContextParams::GetSeed, &LlamaContextParams::SetSeed),
        InstanceAccessor("f16_kv", &LlamaContextParams::GetF16Kv, &LlamaContextParams::SetF16Kv),
        InstanceAccessor("logits_all", &LlamaContextParams::GetLogitsAll, &LlamaContextParams::SetLogitsAll),
        InstanceAccessor("vocab_only", &LlamaContextParams::GetVocabOnly, &LlamaContextParams::SetVocabOnly),
        InstanceAccessor("use_mmap", &LlamaContextParams::GetUseMmap, &LlamaContextParams::SetUseMmap),
        InstanceAccessor("use_mlock", &LlamaContextParams::GetUseMlock, &LlamaContextParams::SetUseMlock),
        InstanceAccessor("embedding", &LlamaContextParams::GetEmbedding, &LlamaContextParams::SetEmbedding)
        // Add similar InstanceAccessor entries for all other properties...
            // called with a progress value between 0 and 1, pass NULL to disable
        // llama_progress_callback progress_callback;
        // // context pointer passed to the progress callback
        // void * progress_callback_user_data;
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("LlamaContextParams", func);
    return exports;
}
// make constructor public
Napi::FunctionReference LlamaContextParams::constructor;

LlamaContextParams::LlamaContextParams(const Napi::CallbackInfo& info) : Napi::ObjectWrap<LlamaContextParams>(info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    // Initialize the internal llama_context_params instance...
    _llama_context_params = new llama_context_params();
    *_llama_context_params = llama_context_default_params();
}

llama_context_params* LlamaContextParams::GetInternalInstance() {
    return _llama_context_params;
}

Napi::Value LlamaContextParams::GetNCtx(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    return Napi::Number::New(env, _llama_context_params->n_ctx);
}
void LlamaContextParams::SetNCtx(const Napi::CallbackInfo& info, const Napi::Value& value) {
    Napi::HandleScope scope(info.Env()); // do I really need a scope for a setter ? getter?
    _llama_context_params->n_ctx = value.As<Napi::Number>().Int32Value();
}

Napi::Value LlamaContextParams::GetNGpuLayers(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), _llama_context_params->n_gpu_layers);
}
void LlamaContextParams::SetNGpuLayers(const Napi::CallbackInfo& info, const Napi::Value& value) {
    _llama_context_params->n_gpu_layers = value.As<Napi::Number>().Int32Value();
}


Napi::Value LlamaContextParams::GetSeed(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), _llama_context_params->seed);
}
void LlamaContextParams::SetSeed(const Napi::CallbackInfo& info, const Napi::Value& value) {
    _llama_context_params->seed = value.As<Napi::Number>().Int32Value();
}


Napi::Value LlamaContextParams::GetF16Kv(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), _llama_context_params->f16_kv);
}
void LlamaContextParams::SetF16Kv(const Napi::CallbackInfo& info, const Napi::Value& value) {
    _llama_context_params->f16_kv = value.As<Napi::Boolean>().Value();
}

Napi::Value LlamaContextParams::GetLogitsAll(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), _llama_context_params->logits_all);
}
void LlamaContextParams::SetLogitsAll(const Napi::CallbackInfo& info, const Napi::Value& value) {
    _llama_context_params->logits_all = value.As<Napi::Boolean>().Value();
}


Napi::Value LlamaContextParams::GetVocabOnly(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), _llama_context_params->vocab_only);
}
void LlamaContextParams::SetVocabOnly(const Napi::CallbackInfo& info, const Napi::Value& value) {
    _llama_context_params->vocab_only = value.As<Napi::Boolean>().Value();
}


Napi::Value LlamaContextParams::GetUseMmap(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), _llama_context_params->use_mmap);
}
void LlamaContextParams::SetUseMmap(const Napi::CallbackInfo& info, const Napi::Value& value) {
    _llama_context_params->use_mmap = value.As<Napi::Boolean>().Value();
}

Napi::Value LlamaContextParams::GetUseMlock(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), _llama_context_params->use_mlock);
}
void LlamaContextParams::SetUseMlock(const Napi::CallbackInfo& info, const Napi::Value& value) {
    _llama_context_params->use_mlock = value.As<Napi::Boolean>().Value();
}

Napi::Value LlamaContextParams::GetEmbedding(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), _llama_context_params->embedding);
}
void LlamaContextParams::SetEmbedding(const Napi::CallbackInfo& info, const Napi::Value& value) {
    _llama_context_params->embedding = value.As<Napi::Boolean>().Value();
}
