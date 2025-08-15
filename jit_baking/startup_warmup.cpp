#include "rpcs3/startup/startup_warmup.h"
#include <thread>

namespace startup
{
	void async_warmup()
	{
		// Preload shaders, cache, and initial modules
		// Example: prime CPU modules, preload textures
	}

	void initialize_async()
	{
		std::thread init_thread(async_warmup);
		init_thread.detach();
	}
} // namespace startup
