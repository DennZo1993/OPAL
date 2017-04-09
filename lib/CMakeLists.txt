include(AddFlagIfSupported)
include(GetCoverageFlags)

# Source files.
set (SOURCES RGBAPixel.cpp
             OPAL.cpp
             OPALSettings.cpp
    )

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

if (UNIX)
  list(APPEND TARGET_LINKER_FLAGS "-lstdc++fs")
endif()

# Sub-directories.
add_subdirectory(ImageIO)
add_subdirectory(util)


add_library(libOPAL STATIC ${SOURCES})

target_compile_options(libOPAL PRIVATE ${TARGET_COMPILER_FLAGS})

if ("${CMAKE_BUILD_TYPE}" MATCHES "Coverage")
  get_coverage_flags(COVERAGE_COMPILER_FLAGS)
  target_compile_options(libOPAL PRIVATE ${COVERAGE_COMPILER_FLAGS})

  list(APPEND TARGET_LINKER_FLAGS "-lgcov" "--coverage")
endif()

# Link.
target_link_libraries(libOPAL libImageIO libUtil ${TARGET_LINKER_FLAGS})

target_include_directories(libOPAL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
