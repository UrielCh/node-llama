export class LlamaContext {
    constructor();

    // Represents the Free method
    free(): void;

    // Represents the InitAll method
    static InitAll(env: any, exports: any): any;
}

// Represents the llama_init_backend function
export function llama_init_backend(): void;

// Represents the llama_print_timings function
export function llama_print_timings(ctx: LlamaContext): void;

// Represents the llama_free function
export function llama_free(ctx: LlamaContext): void;