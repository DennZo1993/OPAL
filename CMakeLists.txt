cmake_minimum_required(VERSION 3.0)

project(OPAL)

if ("${CMAKE_BUILD_TYPE}" MATCHES "Coverage")
  if (NOT UNIX)
    message(FATAL_ERROR "Coverage analysis is only enabled on Unix-systems!")
  endif()
endif()

# Include our CMake functions.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

enable_testing()

add_subdirectory(lib)
add_subdirectory(main)
add_subdirectory(test)
#add_subdirectory(tools/flow2color)
