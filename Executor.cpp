#include <dlfcn.h>
#include <link.h>

#include <KokkosAutoThreads/Core.hpp>
#include <format>
#include <stdexcept>

Executor::Executor(ExecutorConfig config) : config(config) {
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

void Executor::ExecProgram() {
  //
}

void Executor::ExecIteration() {
  for (int t = config.startNumThreads; t <= config.stopNumThreads; t++) {
    ExecProgram();
  }
}

void Executor::Execute() {
  for (int i = 0; i < config.numIterations; i++) {
    ExecIteration();
  }
}


