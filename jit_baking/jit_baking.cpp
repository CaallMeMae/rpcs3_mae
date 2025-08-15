#include "jit_baking.h"
#ifdef ENABLE_JIT_CACHE
#include "rpcs3/jit/jit_cache_manager.h"
#endif
#include <string>
#include <vector>

void jit_baking_placeholder() {}

std::vector<uint8_t> jit_compile_sample(const std::string& fn_key)
{
#ifdef ENABLE_JIT_CACHE
	if (jit::CacheManager::instance().has_cached(fn_key))
	{
		return jit::CacheManager::instance().load_cache(fn_key);
	}
#endif

	std::vector<uint8_t> compiled_code{};

#ifdef ENABLE_JIT_CACHE
	jit::CacheManager::instance().save_cache(fn_key, compiled_code);
#endif
	return compiled_code;
}
