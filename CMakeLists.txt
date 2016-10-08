project(DsExp)

if(NOT MSVC)
  SET (CMAKE_CXX_COMPILER             "clang++")
  SET (CMAKE_CXX_FLAGS                "-std=c++1z -Wall")
endif()
add_executable(DsExp main.cpp SparseMatrix.hpp)