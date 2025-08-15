#pragma once

#include <string>
#include <vector>

void jit_baking_placeholder();
std::vector<uint8_t> jit_compile_sample(const std::string& fn_key);
