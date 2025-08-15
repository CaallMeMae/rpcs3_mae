#pragma once
#include <string>

namespace runtime
{
	std::string detect_game_fingerprint();
	void apply_runtime_config(const std::string& game_id);
} // namespace runtime
