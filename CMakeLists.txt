cmake_minimum_required(VERSION 3.5)
project(DsExp)

# Add project cmake modules to path.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

# We probably don't want this to run on every build.
option(COVERALLS "Generate coveralls data" OFF)

if (COVERALLS)
    include(Coveralls)
    coveralls_turn_on_coverage()
endif()

if(NOT MSVC)
  set(USE_clang false CACHE BOOL "USE clang++")
  if(USE_clang)
    set(CMAKE_CXX_COMPILER             "clang++")
  endif()
  set(CMAKE_CXX_FLAGS                  "-std=c++1z -Wall")
endif()

include_directories(${DsExp_SOURCE_DIR}/src)

add_subdirectory(src)

enable_testing()

add_executable(DsExp main.cpp)

target_link_libraries(DsExp DsExpLib)

add_test(DsExpLib ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/DsExp)