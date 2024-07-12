#include <dlfcn.h>
#include <link.h>
#include <sys/wait.h>

#include <KokkosAutoThreads/Core.hpp>
#include <format>
#include <iostream>
#include <stdexcept>

Executor::Executor(Config config) : config(config) {
  katLibPath = GetFullLibPath(katLibName);
}

std::string Executor::GetFullLibPath(std::string libname) {
  struct link_map *linkMap;

  // Open lib, check it exists
  auto handle = dlopen(katLibName.c_str(), RTLD_LAZY);
  if (handle == NULL) {
    std::string err = std::format("cannot dlopen library {}", katLibName);
    throw std::runtime_error(err);
  }

  // Lib exists, get full path
  auto ret = dlinfo(handle, RTLD_DI_LINKMAP, &linkMap);
  if (ret < 0) {
    std::string err = std::format("cannot dlinfo library {}", katLibName);
    throw std::runtime_error(err);
  }

  return linkMap->l_name;
}

void Executor::ExecProgram(int iter, int numThreads) {
  int pid;

  pid = fork();
  if (pid < 0) {
    std::string err = std::format("cannot fork child. {}", strerror(errno));
    throw std::runtime_error(err);
  }

  if (pid == 0) {
    // Create argv
    auto argv = new char *[config.args.size() + 3];

    // Program path and user-provided arguments
    argv[0] = const_cast<char *>(config.programPath.c_str());
    for (int i = 0; i < config.args.size(); i++) {
      argv[i + 1] = const_cast<char *>(config.args[i].c_str());
    }
    // Num threads arugments
    auto threadArg = std::format("--kokkos-num-threads={}", numThreads);
    argv[config.args.size() + 1] = const_cast<char *>(threadArg.c_str());
    // Kokkos tools
    auto katLibArg = std::format("--kokkos-tools-libs={}", katLibPath);
    argv[config.args.size() + 2] = const_cast<char *>(katLibArg.c_str());
    // NULL terminate
    argv[config.args.size() + 3] = 0;

    int ret;
    ret = setenv(KAT_ENVAR_CONFIG.c_str(), config.configPath.c_str(), 1);
    if (ret < 0) {
      std::string err = std::format("cannot set environment variable {}. {}",
                                    KAT_ENVAR_CONFIG, strerror(errno));
      throw std::runtime_error(err);
    }

    ret = execv(argv[0], argv);
    if (ret < 0) {
      std::string err =
          std::format("child cannot exec program. {}", strerror(errno));
      throw std::runtime_error(err);
    }
  } else {
    int wstatus;
    wait(&wstatus);
    if (wstatus != EXIT_SUCCESS) {
      throw std::runtime_error("child exited unsuccessfully");
    }
  }
}

void Executor::ExecIteration(int iter) {
  for (int t = config.startNumThreads; t <= config.stopNumThreads; t++) {
    ExecProgram(iter, t);
  }
}

void Executor::Execute() {
  for (int i = 0; i < config.numIterations; i++) {
    ExecIteration(i);
  }
}
