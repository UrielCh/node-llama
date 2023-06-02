
const llama = require('./build/Release/node-llama');

/** @type {import('./node-llama.d.ts').LlamaContext} */
const LlamaContext = llama.LlamaContext;

/** @type {import('./node-llama.d.ts').llama_init_backend} */
const initBackend = llama.initBackend;

console.log(Object.keys(llama));
console.log(new LlamaContext());
console.log(initBackend());