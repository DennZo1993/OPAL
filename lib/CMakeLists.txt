set (SOURCES lodepng/lodepng.cpp
             OPAL.cpp)

add_library(libOPAL STATIC ${SOURCES})

if (("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") OR
    ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU"))
  target_compile_options(libOPAL PRIVATE -Wall -Wextra -Werror)
endif()

target_include_directories(libOPAL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
