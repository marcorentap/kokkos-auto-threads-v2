cmake_minimum_required(VERSION 3.16)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 20)

project(KokkosAutoThreads LANGUAGES CXX)

include(FetchContent)


# System packages
find_package(Kokkos REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(MPerf REQUIRED)

# yaml-cpp
FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG yaml-cpp-0.7.0
)
FetchContent_GetProperties(yaml-cpp)
if(NOT yaml-cpp_POPULATED)
  message(STATUS "Fetching yaml-cpp...")
  FetchContent_Populate(yaml-cpp)
  add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()

# add_executable(kokkosautothreads Executor.cpp Analyzer.cpp KokkosAutoThreads.cpp)
add_executable(kokkosautothreads Executor.cpp KokkosAutoThreads.cpp)
target_link_libraries(kokkosautothreads PRIVATE Kokkos::kokkos SQLite::SQLite3)
target_compile_features(kokkosautothreads PRIVATE cxx_std_17)

add_library(kokkosautothreads_lib SHARED LibKokkosAutoThreads.cpp)
set_target_properties(kokkosautothreads_lib PROPERTIES OUTPUT_NAME kokkosautothreads)
target_link_libraries(kokkosautothreads_lib PUBLIC MPerf::MPerf)

target_include_directories(kokkosautothreads SYSTEM
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_include_directories(kokkosautothreads_lib SYSTEM
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

install(TARGETS kokkosautothreads kokkosautothreads_lib)
# install(PROGRAMS 
#   analyzers/kat-average
#   analyzers/kat-plotter
#   analyzers/kat-sort
#   analyzers/kat-misc
#   analyzers/kat-rm-outliers
# DESTINATION bin)

