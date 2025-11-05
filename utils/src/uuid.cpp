#include "utils/uuid.h"

#include <fmt/core.h>

#include <chrono>
#include <cstdint>
#include <random>
#include <thread>

namespace uuid
{

std::string uuidv4()
{
    // 初始化随机种子：rd + 高精度时间戳 + 线程 id 混合
    static std::mt19937_64 gen([] {
        std::random_device rd;
        uint64_t seed = static_cast<uint64_t>(rd()) ^
                        std::chrono::high_resolution_clock::now().time_since_epoch().count() ^
                        std::hash<std::thread::id>{}(std::this_thread::get_id());
        return std::mt19937_64(seed);
    }());
    static std::uniform_int_distribution<uint64_t> dis(0, 0xFFFFFFFFFFFFFFFF);

    uint64_t hi = dis(gen);
    uint64_t lo = dis(gen);
    char buf[37]{};
    auto result = fmt::format_to_n(
        buf, sizeof(buf), "{:08x}-{:04x}-{:04x}-{:04x}-{:012x}", static_cast<uint32_t>(hi >> 32),
        static_cast<uint16_t>((hi >> 16) & 0xFFFF), static_cast<uint16_t>((hi & 0x0FFF) | 0x4000),
        static_cast<uint16_t>((lo >> 48 & 0x3FFF) | 0x8000), static_cast<uint64_t>(lo & 0xFFFFFFFFFFFFULL));
    return std::string(buf, result.out);
}

}  // namespace uuid
