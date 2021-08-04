cmake_minimum_required(VERSION 3.8)

project(runloop
  VERSION 1.0
  DESCRIPTION "runloop c++ library")

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

find_package(Threads REQUIRED)

##################################################
set(runloop_srcs
  ${CMAKE_CURRENT_SOURCE_DIR}/src/loop_imp.cpp)

set(runloop_private_hdrs
  ${CMAKE_CURRENT_SOURCE_DIR}/src/sync.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/thread_manager.h)

set(runloop_public_hdrs
  ${CMAKE_CURRENT_SOURCE_DIR}/include/runloop/loop_imp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/runloop/runloop.h)

set(runloop_all_srcs ${runloop_srcs} ${runloop_private_hdrs} ${runloop_public_hdrs})
set(library_name ${PROJECT_NAME})

# generate .so
add_library(${library_name} SHARED ${runloop_srcs})

target_include_directories(${library_name}
  PUBLIC
    $<INSTALL_INTERFACE:include> # this is for the user's of this package to locate the headers
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(${library_name} PROPERTIES
  LINKER_LANGUAGE CXX
  PUBLIC_HEADER "${runloop_public_hdrs}" # won't work with the double quotes
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  OUTPUT_NAME ${library_name}
)

target_link_libraries(${library_name} PRIVATE Threads::Threads)

# Leave CMAKE_CXX_FLAGS Alone
target_compile_options(${library_name} PRIVATE -Wall -Wextra)
target_compile_features(${library_name} PRIVATE cxx_std_17)

##################################################
# install targets
include(GNUInstallDirs)

# NOTE:
# find_package(lib CONFIG). If you don't do this,
# CMake will first search for a module package (e.g. FindMyLibrary.cmake).
# If it can't find one, *then* it does config package mode.
# You should always use the newer config package of cmake

set(library_target_export "${library_name}-targets")
install(
  TARGETS 	${library_name}
  EXPORT    ${library_target_export}
  ARCHIVE  	DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY  	DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME  	DESTINATION ${CMAKE_INSTALL_BINDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${library_name}
)

install(FILES ${runloop_public_hdrs} DESTINATION DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${library_name})

# install the .cmake of the target
install(
  EXPORT ${library_target_export}
  FILE   runloopConfig.cmake # find_package(runloop CONFIG)
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${library_name}
  NAMESPACE runloop:: # target_link_libraries(target runloop::runloop)
)

##################################################
# ConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/runloopConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/runloopConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${library_name}
)

##################################################
# register package in user's package registry
export(PACKAGE ${PROJECT_NAME})

##################################################
# example on how to use this library
#
# find_package(runloop 1.0 CONFIG)
# if(${runloop_FOUND})
#   message(STATUS "runloop v${runloop_VERSION} found")
#   message(STATUS "runloop directory: ${runloop_DIR}")
# else()
#   message(FATAL_ERROR "runloop not found")
# endif()
# add_executable(temp src/message.cpp)
# target_link_libraries(temp runloop::runloop)

##################################################
##################################################
