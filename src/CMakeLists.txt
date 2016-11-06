cmake_minimum_required(VERSION 3.5)
add_library(DsExpLib Exception.hpp Exception.cpp SparseMatrix.hpp BFS.hpp BFS.cpp ExpressionTree.hpp ExpressionTree.cpp ExpressionStack.hpp ExpressionStack.cpp)

if (COVERALLS)
    set(COVERAGE_SRCS src/Exception.cpp src/SparseMatrix.hpp src/BFS.cpp src/ExpressionTree.cpp src/ExpressionStack.cpp)

    # Create the coveralls target.
    coveralls_setup(
        "${COVERAGE_SRCS}" # The source files.
        ON                 ) # If we should upload.
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

if(ENABLE_SparseMatrix)
  target_compile_definitions(DsExpLib PRIVATE SparseMatrix_enabled)
endif()

if(NOT ENABLE_BFS)
  target_compile_definitions(DsExpLib PRIVATE BFS_disabled)
endif()

if(NOT ENABLE_ExpressionTree)
  target_compile_definitions(DsExpLib PRIVATE ExpressionTree_disabled)
endif()

if(NOT ENABLE_ExpressionStack)
  target_compile_definitions(DsExpLib PRIVATE ExpressionStack_disabled)
endif()