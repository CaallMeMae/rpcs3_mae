add_library(jit_baking STATIC
    jit_baking.cpp
    jit_baking.h
    jit_cache_manager.cpp)

target_include_directories(jit_baking PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include)
