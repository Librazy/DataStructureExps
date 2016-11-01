cmake_minimum_required(VERSION 3.6)
add_executable(DsExp main.cpp SparseMatrix.hpp BFS.hpp BFS.cpp Expression.hpp Expression.cpp)
 
set(ENABLE_BFS true CACHE BOOL "If BFS enabled.")

set(ENABLE_Expression true CACHE BOOL "If Expression enabled. ")

if(MSVC)
  if(CMAKE_GENERATOR_TOOLSET MATCHES "clang")
    set(ENABLE_SparseMatrix true CACHE BOOL "If SparseMatrix enabled. Require compiler with fold expression support")
  else()
    set(ENABLE_SparseMatrix false CACHE BOOL "If SparseMatrix enabled. Require compiler with fold expression support")
  endif()
else()
  set(CMAKE_CXX_FLAGS "-std=c++1z -Wall -g -O0 --coverage")
  set(ENABLE_SparseMatrix true CACHE BOOL "If SparseMatrix enabled. Require compiler with fold expression support")
endif()

if(ENABLE_SparseMatrix)
  target_compile_definitions(DsExp PRIVATE SparseMatrix_enabled)
endif()

if(ENABLE_BFS)
  target_compile_definitions(DsExp PRIVATE BFS_enabled)
endif()

if(ENABLE_Expression)
  target_compile_definitions(DsExp PRIVATE Expression_enabled)
endif()