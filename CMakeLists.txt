cmake_minimum_required(VERSION 3.6)
project(DsExp)

if(NOT MSVC)
  SET (CMAKE_CXX_COMPILER             "clang++")
  SET (CMAKE_CXX_FLAGS                "-std=c++1z -Wall")
endif()
include_directories(${DAY2_SOURCE_DIR}/src)
add_subdirectory(src)