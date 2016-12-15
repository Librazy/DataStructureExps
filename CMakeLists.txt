cmake_minimum_required(VERSION 3.5)
project(DsExp)

# Add project cmake modules to path.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_BUILD_TYPE "Debug")

# We probably don't want this to run on every build.
option(COVERALLS "Generate coveralls data" ON)
option(COVERALLS_UP "Generate coveralls data" OFF)

if(NOT MSVC)
  set(USE_clang false CACHE BOOL "USE clang++")
  if(USE_clang)
    set(CMAKE_CXX_COMPILER             "clang++")
	set(CMAKE_CXX_FLAGS                "-std=c++1z -Wall -g3")
  else()
    set(CMAKE_CXX_FLAGS                "-std=c++1z -Wall -g3 -fno-inline -fno-inline-small-functions -fno-default-inline")
  endif()
endif()

if (COVERALLS)
    include(Coveralls)
    coveralls_turn_on_coverage()
endif()

set(USE_FOLDEXP false CACHE BOOL "If fold expression enabled.")

set(USE_WCOUT false CACHE BOOL "If wcout enabled.")

set(ENABLE_BFS true CACHE BOOL "If BFS enabled.")

set(ENABLE_ExpressionTree true CACHE BOOL "If ExpressionTree enabled.")

set(ENABLE_ExpressionStack true CACHE BOOL "If ExpressionStack enabled.")

set(ENABLE_BinaryTree true CACHE BOOL "If ExpressionStack enabled.")

set(ENABLE_Dijkstra true CACHE BOOL "If Dijkstra enabled. Dependent on SparseMatrix.")

set(ENABLE_Kruskal true CACHE BOOL "If Kruskal enabled. Dependent on SparseMatrix.")

set(ENABLE_SparseMatrix true CACHE BOOL "If SparseMatrix enabled. Require compiler with fold expression support.")

set(ENABLE_AVL true CACHE BOOL "If AVL enabled.")

add_subdirectory(src)

enable_testing()

add_executable(DsExp main.cpp main.h)

if(USE_FOLDEXP)
  target_compile_definitions(DsExp PRIVATE Use_FoldExp)
endif()

if(USE_WCOUT)
  target_compile_definitions(DsExp PRIVATE Use_Wcout)
endif()

if(NOT ENABLE_SparseMatrix)
  target_compile_definitions(DsExp PRIVATE SparseMatrix_disabled)
  set(ENABLE_Dijkstra false CACHE BOOL "If Dijkstra  enabled. Dependent on SparseMatrix" FORCE)
  set(ENABLE_Kruskal false CACHE BOOL "If Kruskal enabled. Dependent on SparseMatrix" FORCE)
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

if(NOT ENABLE_BinaryTree)
  target_compile_definitions(DsExp PRIVATE BinaryTree_disabled)
  set(ENABLE_AVL false CACHE BOOL "If AVL enabled." FORCE)
endif()

if(NOT ENABLE_Dijkstra)
  target_compile_definitions(DsExp PRIVATE Dijkstra_disabled)
endif()

if(NOT ENABLE_Kruskal)
  target_compile_definitions(DsExp PRIVATE Kruskal_disabled)
endif()

if(NOT ENABLE_AVL)
  target_compile_definitions(DsExp PRIVATE AVL_disabled)
endif()

target_link_libraries(DsExp DsExpLib)

add_test(DsExpLib ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/DsExp)
