cmake_minimum_required(VERSION 3.5)
project(DsExp)

# Add project cmake modules to path.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

# We probably don't want this to run on every build.
option(COVERALLS "Generate coveralls data" OFF)

if(NOT MSVC)
  set(USE_clang false CACHE BOOL "USE clang++")
  if(USE_clang)
    set(CMAKE_CXX_COMPILER             "clang++")
  endif()
  set(CMAKE_CXX_FLAGS                  "-std=c++1z -Wall -g3 -fno-inline -fno-inline-small-functions -fno-default-inline")
endif()

if (COVERALLS)
    include(Coveralls)
    coveralls_turn_on_coverage()
endif()

set(ENABLE_BFS true CACHE BOOL "If BFS enabled.")

set(ENABLE_ExpressionTree true CACHE BOOL "If ExpressionTree enabled. ")

set(ENABLE_ExpressionStack true CACHE BOOL "If ExpressionStack enabled. ")

if(MSVC)
  if(CMAKE_GENERATOR_TOOLSET MATCHES "clang")
    set(ENABLE_SparseMatrix true CACHE BOOL "If SparseMatrix enabled. Require compiler with fold expression support")
  else()
    set(ENABLE_SparseMatrix false CACHE BOOL "If SparseMatrix enabled. Require compiler with fold expression support")
  endif()
else()
  set(ENABLE_SparseMatrix true CACHE BOOL "If SparseMatrix enabled. Require compiler with fold expression support")
endif()

include_directories(${DsExp_SOURCE_DIR}/src)

add_subdirectory(src)

enable_testing()

add_executable(DsExp main.cpp)

if(ENABLE_SparseMatrix)
  target_compile_definitions(DsExp PRIVATE SparseMatrix_enabled)
endif()

if(NOT ENABLE_BFS)
  target_compile_definitions(DsExp PRIVATE BFS_disabled)
endif()

if(NOT ENABLE_ExpressionTree)
  target_compile_definitions(DsExp PRIVATE ExpressionTree_disabled)
endif()

if(NOT ENABLE_ExpressionStack)
  target_compile_definitions(DsExp PRIVATE ExpressionStack_disabled)
endif()

target_link_libraries(DsExp DsExpLib)

add_test(DsExpLib ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/DsExp)