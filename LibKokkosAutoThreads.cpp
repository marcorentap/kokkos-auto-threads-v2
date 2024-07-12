#include <stdlib.h>

#include <KokkosAutoThreads/Core.hpp>
#include <MPerf/Backends/CPPChrono.hpp>
#include <MPerf/Core.hpp>
#include <format>
#include <iostream>
#include <stack>
#include <stdexcept>

using vector_of_json = std::vector<MPerf::json>;

Config config;
std::unordered_map<MPerf::Backend *, std::vector<MPerf::Measure *>> measures;

Config LibParseConfigFile(std::string configPath) {
  auto config = Config{.measures = {{"CPPChrono", {"time"}}},
                       .startNumThreads = 1,
                       .stopNumThreads = 4,
                       .numIterations = 3,
                       .databasePath = "kokkosautothreads.db",
                       .tempPath = "kokkosautothreads.tmp",
                       .configPath = configPath};

  for (auto &entry : config.measures) {
    auto backend = entry.first;
    MPerf::Backend *b;
    if (backend == "CPPChrono") {
      b = new MPerf::CPPChrono();
    }

    for (auto &label : entry.second) {
      auto m = b->MakeMeasure(label);
      measures[b].push_back(m);
    }
  }

  return config;
}

inline void ResetAll() {
  for (auto &entry : measures) {
    for (auto &measure : entry.second) {
      measure->Reset();
    }
  }
}

inline void DoMeasureAll() {
  for (auto &entry : measures) {
    for (auto &measure : entry.second) {
      measure->DoMeasure();
    }
  }
}

inline vector_of_json GetJSONAll() {
  vector_of_json jsons;
  for (auto &entry : measures) {
    for (auto &measure : entry.second) {
      jsons.push_back(measure->GetJSON());
    }
  }

  return jsons;
}

// calculare v2 - v1
inline vector_of_json GetJsonDelta(vector_of_json startVec, vector_of_json endVec) {
  vector_of_json vecJDiff;
  // Must be same size...
  if (startVec.size() != endVec.size()) {
    throw std::invalid_argument("v1 and v2 must be the same size");
  }

  for (int i = 0; i < startVec.size(); i++) {
    auto j1 = startVec.at(i);
    auto j2 = endVec.at(i);
    MPerf::json jDiff;

    for (auto &item : j1.items()) {
      auto key = item.key();
      auto v1 = item.value();
      auto v2 = j2[key];
      if (v1.is_number() && v2.is_number()) {
        if (v1.is_number_unsigned() && v2.is_number_unsigned()) {
          uint64_t v1Cast = v1;
          uint64_t v2Cast = v2;
          auto diff = v2Cast - v1Cast;
          jDiff[key] = diff;
          break;
        } else if (v1.is_number_float() && v2.is_number_float()) {
          double v1Cast = v1;
          double v2Cast = v2;
          auto diff = v2Cast - v1Cast;
          jDiff[key] = diff;
          break;
        } else {
          // Only calculate diff of numbers
          int64_t v1Cast = v1;
          int64_t v2Cast = v2;
          auto diff = v2Cast - v1Cast;
          jDiff[key] = diff;
        }

      } else {
        // Not number... just write the values
        jDiff[key] = std::format("{} - {}", to_string(v1), to_string(v2));
      }
    }

    vecJDiff.push_back(jDiff);
  }

  return vecJDiff;
}

std::stack<vector_of_json> measurements;

extern "C" void kokkosp_init_library(const int loadSeq,
                                     const uint64_t interfaceVer,
                                     const uint32_t devInfoCount,
                                     void *deviceInfo) {
  auto backend = MPerf::CPPChrono();
  auto configPath = getenv(KAT_ENVAR_CONFIG.c_str());
  if (configPath == NULL) {
    throw std::runtime_error("cannot get config file path");
  }
  config = LibParseConfigFile(configPath);
}


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
