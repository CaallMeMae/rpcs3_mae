add_library(jit_baking STATIC
    jit_baking.cpp
    jit_baking.h)

target_include_directories(jit_baking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

