#include "LlvmBakeCoordinator.h"
#include <thread>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace rpcs3
{

void LlvmBakeCoordinator::initialize(int max_threads)
{
    thread_count = (max_threads > 0) ? max_threads : static_cast<int>(std::thread::hardware_concurrency());
}

bool LlvmBakeCoordinator::load_or_bake_ir(const std::string& module_id, const std::string& ir_blob, const std::string& flags)
{
    const std::string key = compute_cache_key(module_id, flags);
    const std::string path = "cache/llvm_ir/" + key + ".bc";

    if (fs::exists(path))
    {
        // IR cache hit — skip rebake
        return true;
    }

    // Otherwise, write to cache
    fs::create_directories("cache/llvm_ir/");
    std::ofstream out(path, std::ios::binary);
    if (!out) return false;

    out.write(ir_blob.data(), static_cast<std::streamsize>(ir_blob.size()));
    return out.good();
}

void LlvmBakeCoordinator::parallel_codegen(std::function<void(int)> task, int total_tasks)
{
    std::vector<std::thread> workers;
    int step = (total_tasks + thread_count - 1) / thread_count;

    for (int i = 0; i < thread_count; ++i)
    {
        workers.emplace_back([=]() {
            for (int j = i * step; j < std::min((i + 1) * step, total_tasks); ++j)
            {
                task(j);
            }
        });
    }

    for (auto& w : workers) w.join();
}

std::string LlvmBakeCoordinator::compute_cache_key(const std::string& module_id, const std::string& flags) const
{
    // Naive hash for now — replace with proper xxhash or SHA in the future
    return module_id + "_" + std::to_string(std::hash<std::string>{}(flags));
}

}
