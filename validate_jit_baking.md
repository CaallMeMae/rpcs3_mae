# JIT Baking Integration Validation

```bash
# Ensure jit_baking subdirectory is included
rg "add_subdirectory\(jit_baking\)" -n CMakeLists.txt

# Verify module CMakeLists exists
[ -f jit_baking/CMakeLists.txt ] && echo "jit_baking CMakeLists present"

# Check rpcs3_lib links against jit_baking
rg "target_link_libraries\(rpcs3_lib" -n -A20 rpcs3/CMakeLists.txt | rg "jit_baking" -n
```
