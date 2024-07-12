#include <KokkosAutoThreads/Core.hpp>
#include <KokkosAutoThreads/Lib/ArgParse.hpp>
#include <exception>

int main(int argc, char *argv[]) {
  std::string configFilePath = "";
  std::string programPath;
  Config config;

  // Parse arguments
  argparse::ArgumentParser program("kokkosautothreads");
  program.add_argument("--config")
      .help("specify the config file")
      .store_into(configFilePath);
  program.add_argument("program_path")
      .help("specify the program to execute")
      .store_into(programPath);
  program.add_argument("args").help("specify program arguments").remaining();
  try {
    program.parse_args(argc, argv);

    config = ParseConfigFile(configFilePath);

    // Program execution
    config.programPath = programPath;
    if (program.present("args"))
      config.args = program.get<std::vector<std::string>>("args");
    else
      config.args = {};
  } catch (std::exception &e) {
    std::cerr << e.what();
    std::cerr << program;
  }

  // Execute
  auto exec = Executor(config);
  exec.Execute();
}
