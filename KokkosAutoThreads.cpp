#include <KokkosAutoThreads/Core.hpp>
#include <KokkosAutoThreads/Lib/ArgParse.hpp>
#include <exception>
#include <filesystem>
#include <format>
#include <stdexcept>

std::string configFilePath;

Config ParseConfigFile(std::string configPath) {
  return Config{.measures = {{"cppchrono", {"time"}}},
                .startNumThreads = 1,
                .stopNumThreads = 4,
                .numIterations = 3,
                .databasePath = "kokkosautothreads.db",
                .tempPath = "kokkosautothreads.tmp"};
}

int main(int argc, char *argv[]) {
  // Parse arguments
  argparse::ArgumentParser program("kokkosautothreads");
  program.add_argument("--config")
      .default_value("kokkosautothreads.yml")
      .help("specify the config file")
      .store_into(configFilePath);
  try {
    program.parse_args(argc, argv);

    // Config file must exist
    if (!std::filesystem::exists(configFilePath)) {
      std::string err = std::format("{} doesn't exist. ", configFilePath);
      throw std::invalid_argument(err);
    }

  } catch (std::exception &e) {
    std::cerr << e.what();
    std::cerr << program;
  }
}
