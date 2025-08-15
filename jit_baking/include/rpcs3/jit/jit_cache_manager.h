#pragma once
#ifdef ENABLE_JIT_CACHE

#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

namespace jit
{
	class CacheManager
	{
	public:
		static CacheManager& instance();

		void initialize(const std::string& cache_dir);
		bool has_cached(const std::string& function_key) const;
		std::vector<uint8_t> load_cache(const std::string& function_key);
		void save_cache(const std::string& function_key, const std::vector<uint8_t>& compiled_code);

	private:
		std::string m_cache_directory;
		std::unordered_map<std::string, std::vector<uint8_t>> m_cache_table;

		CacheManager() = default;
		void load_index();
		void write_index();
	};
} // namespace jit

#endif
