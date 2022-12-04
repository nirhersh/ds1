cmake_minimum_required(VERSION 3.0.0)
project(ds-test VERSION 0.0.1)
set(CMAKE_BUILD_TYPE Debug)
include(CTest)
enable_testing()

add_executable(ds-test tests\\wcTest.cpp Player.cpp Team.cpp Exception.h wet1util.h worldcup23a1.cpp AVLTree.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(Cpack)