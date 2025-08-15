#include "stdafx.h"
#include "rpcs3.h"
#ifdef ENABLE_JIT_CACHE
#include "rpcs3/startup/startup_warmup.h"
#endif
#ifdef ENABLE_SMART_RUNTIME
#include "rpcs3/runtime/runtime_fingerprint.h"
#endif

LOG_CHANNEL(sys_log, "SYS");

int main(int argc, char** argv)
{
#ifdef ENABLE_JIT_CACHE
	startup::initialize_async();
#endif
#ifdef ENABLE_SMART_RUNTIME
	auto id = runtime::detect_game_fingerprint();
	runtime::apply_runtime_config(id);
#endif
	const int exit_code = run_rpcs3(argc, argv);
	sys_log.notice("RPCS3 terminated with exit code %d", exit_code);
	return exit_code;
}
