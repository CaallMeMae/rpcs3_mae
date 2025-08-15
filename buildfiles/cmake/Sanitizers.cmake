# Sanitizers and ThinLTO integration

macro(enable_project_sanitizers target_name)
    if(MSVC)
        message(STATUS "Sanitizers not supported on MSVC.")
        return()
    endif()

    if(ENABLE_ASAN)
        target_compile_options(${target_name} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(${target_name} PRIVATE -fsanitize=address)
    endif()

    if(ENABLE_UBSAN)
        target_compile_options(${target_name} PRIVATE -fsanitize=undefined -fno-omit-frame-pointer)
        target_link_options(${target_name} PRIVATE -fsanitize=undefined)
    endif()

    if(ENABLE_TSAN)
        target_compile_options(${target_name} PRIVATE -fsanitize=thread)
        target_link_options(${target_name} PRIVATE -fsanitize=thread)
    endif()

    if(ENABLE_THINLTO)
        target_compile_options(${target_name} PRIVATE -flto=thin)
        target_link_options(${target_name} PRIVATE -flto=thin)
    endif()
endmacro()
