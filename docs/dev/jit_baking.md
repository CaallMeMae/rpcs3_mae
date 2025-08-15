# LLVM JIT Baking Optimization

This system introduces a persistent IR cache and parallel codegen manager.

## Features

- IR blobs saved to disk (no recompilation)
- Parallel LLVM IR → Object translation
- Precomputed per-title cache keys
- Compatible with ThinLTO

## Cache Path

cache/llvm_ir/<module_hash>.bc


## Coming Up

- ThinLTO codegen toggles
- Prewarm for hot blocks (per-title profiles)
- Shader pipeline integration
