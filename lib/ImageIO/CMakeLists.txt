set (SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../lodepng/lodepng.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../RGBAPixel.cpp
             AnalyzeHeader.cpp
             ImageReader.cpp
             PngImageWriter.cpp)

add_library(libImageIO STATIC ${SOURCES})

if (("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") OR
    ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU"))
  target_compile_options(libImageIO PRIVATE
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


target_include_directories(libImageIO PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
