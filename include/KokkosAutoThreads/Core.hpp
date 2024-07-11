#ifndef KOKKOSAUTOTHREADS_HPP
#define KOKKOSAUTOTHREADS_HPP

#include <KokkosAutoThreads/Lib/Json.hpp>
#include <MPerf/Core.hpp>

struct Config {
  std::unordered_map<std::string, std::vector<std::string>> measures;
  int startNumThreads;
  int stopNumThreads;
  int numIterations;
  std::string databasePath;
  std::string tempPath;
};

static Config ParseConfigFile(std::string configPath);

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
  void ExecIteration();
  void ExecProgram();

 public:
  Executor(Config config);
  void Execute();
};

#endif
