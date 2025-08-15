# JIT Baking Integration Validation

```bash
# Ensure jit_baking subdirectory is included
rg "add_subdirectory\(jit_baking\)" -n CMakeLists.txt

# Verify module CMakeLists exists
[ -f jit_baking/CMakeLists.txt ] && echo "jit_baking CMakeLists present"

# Check rpcs3_lib links against jit_baking
rg "target_link_libraries\(rpcs3_lib" -n -A20 rpcs3/CMakeLists.txt | rg "jit_baking" -n

# Ensure jit_cache_manager.cpp is part of build
rg "jit_cache_manager.cpp" -n jit_baking/CMakeLists.txt

# Build test to confirm CacheManager::instance links
cat <<'CPP' > /tmp/cache_test.cpp
#include "rpcs3/jit/jit_cache_manager.h"
int main() { return &jit::CacheManager::instance(), 0; }
CPP
c++ -std=c++20 /tmp/cache_test.cpp -Ijit_baking/include -c && echo "CacheManager links"
```
