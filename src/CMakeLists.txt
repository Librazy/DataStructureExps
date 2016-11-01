cmake_minimum_required(VERSION 3.6)
add_library(DsExpLib SparseMatrix.hpp BFS.hpp BFS.cpp Expression.hpp Expression.cpp)

if (COVERALLS)
    set(COVERAGE_SRCS src/SparseMatrix.hpp src/BFS.hpp src/BFS.cpp src/Expression.hpp src/Expression.cpp)

    # Create the coveralls target.
    coveralls_setup(
        "${COVERAGE_SRCS}" # The source files.
        ON                 ) # If we should upload.
endif()

enable_testing()

set(ENABLE_BFS true CACHE BOOL "If BFS enabled.")

set(ENABLE_Expression true CACHE BOOL "If Expression enabled. ")

if(MSVC)
  if(CMAKE_GENERATOR_TOOLSET MATCHES "clang")
    set(ENABLE_SparseMatrix true CACHE BOOL "If SparseMatrix enabled. Require compiler with fold expression support")
  else()
    set(ENABLE_SparseMatrix false CACHE BOOL "If SparseMatrix enabled. Require compiler with fold expression support")
  endif()
else()
  set(ENABLE_SparseMatrix true CACHE BOOL "If SparseMatrix enabled. Require compiler with fold expression support")
endif()

if(ENABLE_SparseMatrix)
  target_compile_definitions(DsExpLib PRIVATE SparseMatrix_enabled)
endif()

if(ENABLE_BFS)
  target_compile_definitions(DsExpLib PRIVATE BFS_enabled)
endif()

if(ENABLE_Expression)
  target_compile_definitions(DsExpLib PRIVATE Expression_enabled)
endif()

add_executable(DsExp main.cpp)

target_link_libraries(DsExp DsExpLib)

add_test(DsExpLib ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/DsExp)