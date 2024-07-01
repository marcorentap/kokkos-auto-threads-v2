#ifndef KOKKOSAUTOTHREADS_HPP
#define KOKKOSAUTOTHREADS_HPP

#include <KokkosAutoThreads/Lib/Json.hpp>
#include <MPerf/Core.hpp>

class Executor {
  using json = nlohmann::json;

 private:
  struct ExecutorConfig {
    std::unordered_map<std::string, std::vector<std::string>> measures;
    int startNumThreads;
    int stopNumThreads;
    int numIterations;
    std::string databasePath;
    std::string tempPath;
  } config;
  std::string GetFullLibPath();

  std::string katLibName = "libkokkosautothreads.so";
  std::string katLibPath = ""; // Absolute path to lib
  //   static std::string progLogName = "kokkosautothreads.tmp.json";
  // static std::string logName = "kokkosautothreads.json";
  // static std::string libName = "libkokkosautothreads.so";
  // static std::string summaryName = "kokkosautothreads.summary.json";
  // static std::string dbName = "kokkosautothreads.db";

  std::string GetFullLibPath(std::string libname);
  void ExecIteration();
  void ExecProgram();

 public:
  Executor(ExecutorConfig config);
  void Execute();
  static ExecutorConfig ParseConfigFile(std::string configPath);
};

#endif
