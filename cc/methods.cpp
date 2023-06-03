#include "methods.hh"

static Napi::Value initFromFile(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected two arguments").ThrowAsJavaScriptException();
        return env.Null();
    }
    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Expected first argument to be a string").ThrowAsJavaScriptException();
        return env.Null();
    }
    if (!info[1].IsObject()) {
        Napi::TypeError::New(env, "Expected second argument to be an object").ThrowAsJavaScriptException();
        return env.Null();
    }
    Napi::String path_model = info[0].As<Napi::String>();
    Napi::Object contex_obj = info[1].As<Napi::Object>();
    LlamaContextParams* context = Napi::ObjectWrap<LlamaContextParams>::Unwrap(contex_obj);
    struct llama_context* result = llama_init_from_file(path_model.Utf8Value().c_str(), context->_llama_context_params);

    // Encapsulate the result in a LlamaContext
    LlamaContext* llamaContext = new LlamaContext(info, result);

    // TODO wrap llamaContext as an Napi::Object
    
    // return LlamaContext::ObjectWrap(llamaContext);
    // Create a new instance of JavaScript LlamaContext object
    // Napi::Object obj = LlamaContext::constructor.New({ Napi::External<LlamaContext>::New(env, llamaContext) });

    // Return the new instance
    // return obj;
}

