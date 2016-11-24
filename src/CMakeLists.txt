cmake_minimum_required(VERSION 3.5)
add_library(DsExpLib Exception.h Exception.cpp SparseMatrix.hpp BFS.h BFS.cpp ExpressionTree.h ExpressionTree.cpp ExpressionStack.h ExpressionStack.cpp BinaryTree.hpp Dijkstra.h Dijkstra.cpp)

if (COVERALLS)
    set(COVERAGE_SRCS src/Exception.cpp src/SparseMatrix.hpp src/BFS.cpp src/ExpressionTree.cpp src/ExpressionStack.cpp src/BinaryTree.hpp src/Dijkstra.h src/Dijkstra.cpp)

    # Create the coveralls target.
    coveralls_setup(
        "${COVERAGE_SRCS}" # The source files.
        ${COVERALLS_UP}     ) # If we should upload.
endif()

if(USE_FOLDEXP)
  target_compile_definitions(DsExpLib PRIVATE Use_FoldExp)
endif()

if(USE_WCOUT)
  target_compile_definitions(DsExpLib PRIVATE Use_Wcout)
endif()

if(NOT ENABLE_SparseMatrix)
  target_compile_definitions(DsExpLib PRIVATE SparseMatrix_disabled)
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