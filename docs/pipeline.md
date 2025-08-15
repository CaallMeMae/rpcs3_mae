# Development Pipeline – Modern Manager

This branch introduces a structured quality pipeline designed to support long-term stability and contribution scale.

### Included
- Cross-platform GitHub Actions build matrix
- Sanitizer toggles (ASAN, UBSAN, TSAN)
- Clang-tidy, clang-format, and editor config
- Golden seed scaffolding for runtime regressions

### Coming Next
- LLVM JIT improvements
- Shader cache optimization
- Faster startup and async UX improvements
- Save-state validation loops
- Compatibility sweeps and syscall coverage

### Contribution Notes
- New features must respect the existing config and toolchain rules.
- Formatting changes should be separated from logic changes to preserve diff clarity.
- Configuration should be wrapped in optional flags unless performance-critical.
