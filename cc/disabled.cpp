
Napi::Number ModelQuantizeWrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 4) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
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
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
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

