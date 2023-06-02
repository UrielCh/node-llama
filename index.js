const llama = require('./build/Release/node-llama');

console.log(llama);
console.log(new llama.LlamaContext());
console.log(llama.initBackend());

