# Sanitizers & ThinLTO Support

This project supports optional developer flags for memory and thread debugging, as well as link-time optimization.

## How to Use (Local Build)

Example for Linux/macOS:

```bash
cmake -S . -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DENABLE_ASAN=ON \
  -DENABLE_UBSAN=ON \
  -DENABLE_THINLTO=ON
