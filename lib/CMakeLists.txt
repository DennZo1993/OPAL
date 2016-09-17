set (SOURCES lodepng/lodepng.cpp
             OPAL.cpp)

add_library(libOPAL STATIC ${SOURCES})

if (("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") OR
    ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU"))
  target_compile_options(libOPAL PRIVATE
                         -std=c++11
                         -Werror
                         -Wall -Wextra
                         -Wfloat-equal
                         -Wshadow
                         -Wpointer-arith
                         -Wcast-align
                         -Wswitch-enum
                         -Wuninitialized)
endif()

target_include_directories(libOPAL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
