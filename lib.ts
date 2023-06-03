export interface LlamaContext {
  // Represents the Free method
  free(): void;

  // Represents the tokenNL method
  tokenNL(): number;

  // Represents the tokenEOS method
  tokenEOS(): number;

  // Represents the tokenBOS method
  tokenBOS(): number;
}

export interface LlamaContextConstructor {
  new (): LlamaContext;
}

export interface LlamaContextParams {
  n_ctx: number;
  // n_vocab: number;
  // n_embd: number;
  // n_head: number;
  // n_layer: number;
  n_gpu_layers: number;
  seed: number;
  f16_kv: boolean;
  logits_all: boolean;
  vocab_only: boolean;
  use_mmap: boolean;
  use_mlock: boolean;
  embedding: boolean;
}

export interface LlamaContextParamsConstructor {
  new (): LlamaContextParams;
}

export interface Llamalib {
  LlamaContext: LlamaContextConstructor;
  LlamaContextParams: LlamaContextParamsConstructor;

  initFromFile(filename: string, params: LlamaContextParams): LlamaContext | null; 
}
