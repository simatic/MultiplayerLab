#pragma once

#ifdef TRACY_ENABLE
#include <Tracy.hpp>

#ifdef PROFILING_IMPLEMENTATION
void* operator new(std::size_t count) {
    auto ptr = malloc(count);
    TracySecureAllocS(ptr, count, 20);
    return ptr;
}

void operator delete(void* ptr) noexcept{
    TracySecureFreeS(ptr, 20);
    free(ptr);
}
#endif

#else
// Empty defines
#define FrameMark
#define ZoneScoped
#define ZoneScopedN
#define TracyMessageL
#endif