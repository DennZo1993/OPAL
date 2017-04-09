include(AddFlagIfSupported)
include(GetCoverageFlags)

set (SOURCES fs/File.cpp
             fs/Directory.cpp
             json/json.cpp
             string/Split.cpp)

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

# Linker flags.
set(TARGET_LINKER_FLAGS "")

# Sub-directories.
add_subdirectory(json/json11)


add_library(libUtil STATIC ${SOURCES})

target_compile_options(libUtil PRIVATE ${TARGET_COMPILER_FLAGS})

if ("${CMAKE_BUILD_TYPE}" MATCHES "Coverage")
  get_coverage_flags(COVERAGE_COMPILER_FLAGS)
  target_compile_options(libUtil PRIVATE ${COVERAGE_COMPILER_FLAGS})

  list(APPEND TARGET_LINKER_FLAGS "-lgcov" "--coverage")
endif()


target_link_libraries(libUtil json11 ${TARGET_LINKER_FLAGS})

target_include_directories(libUtil PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
