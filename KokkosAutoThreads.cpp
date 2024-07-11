#include <KokkosAutoThreads/Core.hpp>
#include <KokkosAutoThreads/Lib/ArgParse.hpp>
#include <exception>
#include <filesystem>
#include <format>
#include <stdexcept>

std::string configFilePath;

Config ParseConfigFile(std::string configPath) {
  return Config{
      .measures = {{"cppchrono", {"time"}}},
      .startNumThreads = 1,
      .stopNumThreads = 4,
      .numIterations = 3,
      .databasePath = "kokkosautothreads.db",
      .tempPath = "kokkosautothreads.tmp",
  };
}

int main(int argc, char *argv[]) {
  std::string programPath;
  Config config;

  // Parse arguments
  argparse::ArgumentParser program("kokkosautothreads");
  program.add_argument("--config")
      .default_value("kokkosautothreads.yml")
      .help("specify the config file")
      .store_into(configFilePath);
  program.add_argument("program_path")
      .help("specify the program to execute")
      .store_into(programPath);
  program.add_argument("args").help("specify program arguments").remaining();
  try {
    program.parse_args(argc, argv);

    // Config file
    if (!std::filesystem::exists(configFilePath)) {
      std::string err = std::format("{} doesn't exist. ", configFilePath);
      throw std::invalid_argument(err);
    }
    config = ParseConfigFile(configFilePath);

    // Program execution
    config.programPath = programPath;
    config.args = program.get<std::vector<std::string>>("args");

  } catch (std::exception &e) {
    std::cerr << e.what();
    std::cerr << program;
  }

  // Execute
  auto exec = Executor(config);
  exec.Execute();
}
