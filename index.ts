import { Llamalib } from "./lib";

// Represents the llama_init_backend function
// export function llama_init_backend(): void;

// Represents the llama_print_timings function
// export function llama_print_timings(ctx: LlamaContext): void;

// Represents the llama_free function
// export function llama_free(ctx: LlamaContext): void;

const llama = require("./build/Release/node-llama") as Llamalib;

const params = new llama.LlamaContextParams();

// const LlamaContext = llama.LlamaContext;

const paramsFileds = [
  "n_ctx",
  "n_vocab",
  "n_embd",
  "n_head",
  "n_layer",
  "n_gpu_layers",
  "seed",
  "f16_kv",
  "logits_all",
  "vocab_only",
  "use_mmap",
  "use_mlock",
  "embedding",
] as const;

console.log(Object.keys(llama));
for (const p of paramsFileds) {
  console.log(`${p}: ${params[p]}`);
}

params.n_embd = 10;
console.log('update: n_embd', params.n_embd);
// const context = new LlamaContext();
// console.log(context);
// console.log('getPrototypeOf:', Object.getPrototypeOf(context));
// console.log('constructor:', context.constructor);
// console.log('prototype:', context.constructor.prototype);
// console.log('prototype.constructor:', context.constructor.prototype.constructor);
//
// // console.log('getKvCacheTokenCount:', context.getKvCacheTokenCount());
// // console.log('getStateSize:', context.getStateSize());
// console.log('tokenBOS:', context.tokenBOS());
// console.log('tokenEOS:', context.tokenEOS());
// console.log('tokenNL:', context.tokenNL());
// // Object.getPrototypeOf(context));
//
// console.log('mmapSupported:', llama.mmapSupported());
// console.log("mlockSupported:", llama.mlockSupported());
//
