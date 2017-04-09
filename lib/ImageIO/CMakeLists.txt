include(AddFlagIfSupported)
include(GetCoverageFlags)

set (SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../lodepng/lodepng.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../RGBAPixel.cpp
             AnalyzeHeader.cpp
             PngImageWriter.cpp)

# Compiler flags for this target
add_flag_if_supported("-std=c++11"      TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wall"           TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wextra"         TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wshadow"        TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wpointer-arith" TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wcast-align"    TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wswitch-enum"   TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wuninitialized" TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Werror"         TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wfloat-equal"   TARGET_COMPILER_FLAGS)


add_library(libImageIO STATIC ${SOURCES})

target_compile_options(libImageIO PRIVATE ${TARGET_COMPILER_FLAGS})

if ("${CMAKE_BUILD_TYPE}" MATCHES "Coverage")
  get_coverage_flags(COVERAGE_COMPILER_FLAGS)
  target_compile_options(libImageIO PRIVATE ${COVERAGE_COMPILER_FLAGS})
endif()


target_include_directories(libImageIO PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
