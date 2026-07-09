#include <storage.hpp>

uint32_t (&Storage::scratchpad)[256] = *(uint32_t(*)[256])0x1F800000; // 1кб

int Storage::InitializeScratchpad() {
    return 0;
}