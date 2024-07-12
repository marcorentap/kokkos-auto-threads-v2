#ifndef KOKKOSAUTOTHREADS_HPP
#define KOKKOSAUTOTHREADS_HPP

#include <KokkosAutoThreads/Lib/Json.hpp>
#include <MPerf/Backends/CPPChrono.hpp>
#include <MPerf/Core.hpp>

const std::string KAT_ENVAR_CONFIG = "KAT_ENVAR_CONFIG";

struct Config {
  std::unordered_map<std::string, std::vector<std::string>> measures;
  int startNumThreads;
  int stopNumThreads;
  int numIterations;
  std::string databasePath;
  std::string tempPath;
  std::string programPath;
  std::vector<std::string> args;
  std::string configPath;
};

static Config ParseConfigFile(std::string configPath) {
  auto config = Config{
      .measures = {{"cppchrono", {"time"}}},
      .startNumThreads = 1,
      .stopNumThreads = 4,
      .numIterations = 3,
      .databasePath = "kokkosautothreads.db",
      .tempPath = "kokkosautothreads.tmp",
      .configPath = configPath
  };

  return config;
}


class Executor {
  using json = nlohmann::json;

 private:
  std::string GetFullLibPath();
  Config config;

  std::string katLibName = "libkokkosautothreads.so";
  std::string katLibPath = "";  // Absolute path to lib
  //   static std::string progLogName = "kokkosautothreads.tmp.json";
  // static std::string logName = "kokkosautothreads.json";
  // static std::string libName = "libkokkosautothreads.so";
  // static std::string summaryName = "kokkosautothreads.summary.json";
  // static std::string dbName = "kokkosautothreads.db";

  std::string GetFullLibPath(std::string libname);
  void ExecProgram(int iter, int numThreads);
  void ExecIteration(int iter);

 public:
  Executor(Config config);
  void Execute();
};

#endif
