cmake_minimum_required(VERSION 3.8)

option(build_type_debug "Build Debug" ON)
option(generate_compile_commands "Generate compile_commands.json" ON)

if(${build_type_debug})
  set(CMAKE_BUILD_TYPE Debug)
  message("==> Build Type: Debug")
  message("==> Debug Symbols Generated")
else()
  set(CMAKE_BUILD_TYPE Release)
  message("==> Build Type: Release")
endif()

# generate compile_commands.json
if(${generate_compile_commands})
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()


find_package(runloop 1.0 CONFIG REQUIRED)
if(${runloop_FOUND})
  message(STATUS "runloop v${runloop_VERSION} found")
  message(STATUS "runloop directory: ${runloop_DIR}")
else()
  message(FATAL_ERROR "runloop not found")
endif()

find_package(Threads REQUIRED)

add_executable(test_runloop src/test_runloop.cpp)
target_link_libraries(test_runloop
  runloop::runloop
  Threads::Threads)
target_compile_options(test_runloop PRIVATE -Wall -Wextra)
target_compile_features(test_runloop PRIVATE cxx_std_17)
