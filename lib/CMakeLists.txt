set (Matrix_SRCS Matrix.h Matrix.cpp)
set (ImageDatabase_SRCS ImageDatabase.h ImageDatabase.cpp)
set (lodepng_SRCS lodepng/lodepng.h lodepng/lodepng.cpp)

add_library(libOPAL STATIC
            ${Matrix_SRCS}
            ${ImageDatabase_SRCS}
            ${lodepng_SRCS})

if (("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") OR
    ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU"))
  target_compile_options(libOPAL PRIVATE -Wall -Wextra -Werror)
endif()

target_include_directories(libOPAL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
