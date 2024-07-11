#include <KokkosAutoThreads/Core.hpp>

extern "C" void kokkosp_init_library(const int loadSeq,
                                     const uint64_t interfaceVer,
                                     const uint32_t devInfoCount,
                                     void *deviceInfo) {}

extern "C" void kokkosp_finalize_library() {}

extern "C" void kokkosp_begin_parallel_for(const char *name,
                                           const uint32_t devID,
                                           uint64_t *kID) {}

extern "C" void kokkosp_end_parallel_for(const uint64_t kID) {}

extern "C" void kokkosp_begin_parallel_scan(const char *name,
                                            const uint32_t devID,
                                            uint64_t *kID) {}

extern "C" void kokkosp_end_parallel_scan(const uint64_t kID) {}

extern "C" void kokkosp_begin_parallel_reduce(const char *name,
                                              const uint32_t devID,
                                              uint64_t *kID) {}

extern "C" void kokkosp_end_parallel_reduce(const uint64_t kID) {}
