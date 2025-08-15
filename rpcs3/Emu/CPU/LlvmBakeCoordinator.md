#pragma once

#include <string>
#include <unordered_map>
#include <functional>

namespace rpcs3
{
class LlvmBakeCoordinator
{
public:
    void initialize(int max_threads = -1);
    bool load_or_bake_ir(const std::string& module_id, const std::string& ir_blob, const std::string& flags);
    void parallel_codegen(std::function<void(int index)> task, int total_tasks);
    std::string compute_cache_key(const std::string& module_id, const std::string& flags) const;

private:
    int thread_count = 1;
};
}
