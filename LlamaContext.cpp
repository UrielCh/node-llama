#include "LlamaContext.hh"

Napi::FunctionReference LlamaContext::constructor;

Napi::Object LlamaContext::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "LlamaContext", {
        InstanceMethod("free", &LlamaContext::Free),
        InstanceMethod("copyStateData", &LlamaContext::CopyStateData),
        InstanceMethod("setStateData", &LlamaContext::SetStateData),
        InstanceMethod("loadSessionFile", &LlamaContext::LoadSessionFile),
        InstanceMethod("saveSessionFile", &LlamaContext::SaveSessionFile),
        InstanceMethod("eval", &LlamaContext::Eval),

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

Napi::Value LlamaContext::CopyStateData(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Check if there is one argument and if it is a Buffer
    if (info.Length() != 1 || !info[0].IsBuffer()) {
        Napi::TypeError::New(env, "A single Buffer argument is expected").ThrowAsJavaScriptException();
    }

    // Get the Buffer from the arguments
    Napi::Buffer<uint8_t> buffer = info[0].As<Napi::Buffer<uint8_t>>();

    // Call the C++ function and get the result
    size_t result = llama_copy_state_data(_llamaCtx, buffer.Data());

    // Return the result as a Number
    return Napi::Number::New(env, result);
}

Napi::Value LlamaContext::SetStateData(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    // Check if there is one argument and if it is a Buffer
    if (info.Length() != 1 || !info[0].IsBuffer()) {
        Napi::TypeError::New(env, "A single Buffer argument is expected").ThrowAsJavaScriptException();
    }

    // Get the Buffer from the arguments
    Napi::Buffer<uint8_t> buffer = info[0].As<Napi::Buffer<uint8_t>>();

    // Call the C++ function and get the result
    size_t result = llama_set_state_data(_llamaCtx, buffer.Data());

    // Return the result as a Number
    return Napi::Number::New(env, result);
}

Napi::Value LlamaContext::LoadSessionFile(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    // Check if there are enough arguments and if they are of the correct type
    if (info.Length() < 3 || !info[0].IsString() || !info[1].IsBuffer() || !info[2].IsNumber()) {
        Napi::TypeError::New(env, "Expected arguments: string, Buffer, number").ThrowAsJavaScriptException();
    }

    std::string path_session = info[0].As<Napi::String>().Utf8Value();
    Napi::Buffer<llama_token> tokens_out = info[1].As<Napi::Buffer<llama_token>>();
    size_t n_token_capacity = info[2].As<Napi::Number>().Uint32Value();

    size_t n_token_count_out;

    // Call the C++ function and get the result
    bool result = llama_load_session_file(_llamaCtx, path_session.c_str(), tokens_out.Data(), n_token_capacity, &n_token_count_out);

    // Return the result as a Boolean
    return Napi::Boolean::New(env, result);
}

Napi::Value LlamaContext::SaveSessionFile(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    // Check if there are enough arguments and if they are of the correct type
    if (info.Length() < 3 || !info[0].IsString() || !info[1].IsBuffer() || !info[2].IsNumber()) {
        Napi::TypeError::New(env, "Expected arguments: string, Buffer, number").ThrowAsJavaScriptException();
    }

    std::string path_session = info[0].As<Napi::String>().Utf8Value();
    Napi::Buffer<llama_token> tokens = info[1].As<Napi::Buffer<llama_token>>();
    size_t n_token_count = info[2].As<Napi::Number>().Uint32Value();

    // Call the C++ function and get the result
    bool result = llama_save_session_file(_llamaCtx, path_session.c_str(), tokens.Data(), n_token_count);

    // Return the result as a Boolean
    return Napi::Boolean::New(env, result);
}

Napi::Value LlamaContext::Eval(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    // Check if there are enough arguments and if they are of the correct type
    if (info.Length() < 4 || !info[0].IsBuffer() || !info[1].IsNumber() || !info[2].IsNumber() || !info[3].IsNumber()) {
        Napi::TypeError::New(env, "Expected arguments: Buffer, number, number, number").ThrowAsJavaScriptException();
    }

    Napi::Buffer<llama_token> tokens = info[0].As<Napi::Buffer<llama_token>>();
    int n_tokens = info[1].As<Napi::Number>().Int32Value();
    int n_past = info[2].As<Napi::Number>().Int32Value();
    int n_threads = info[3].As<Napi::Number>().Int32Value();

    // Call the C++ function and get the result
    int result = llama_eval(_llamaCtx, tokens.Data(), n_tokens, n_past, n_threads);

    // Return the result as a Number
    return Napi::Number::New(env, result);
}