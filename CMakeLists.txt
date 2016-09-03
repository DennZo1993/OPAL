cmake_minimum_required(VERSION 3.0)

project(OPAL)

if (("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") OR
    ("${CMAKE_CXX_COMPILER_IS}" MATCHES "GNU"))
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

enable_testing()

add_subdirectory(main)
add_subdirectory(test)
add_subdirectory(lib)
