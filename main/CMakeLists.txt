include(AddFlagIfSupported)
include(GetCoverageFlags)

set(Main_SRCS main.cpp)

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

set(TARGET_LINKER_FLAGS "")

add_executable(OPAL ${Main_SRCS})

target_compile_options(OPAL PRIVATE ${TARGET_COMPILER_FLAGS})

if ("${CMAKE_BUILD_TYPE}" MATCHES "Coverage")
  get_coverage_flags(COVERAGE_COMPILER_FLAGS)
  target_compile_options(libOPAL PRIVATE ${COVERAGE_COMPILER_FLAGS})

  list(APPEND TARGET_LINKER_FLAGS "-lgcov" "--coverage")
endif()

target_include_directories(OPAL PRIVATE ../lib ../lib/ImageIO)
target_link_libraries(OPAL libOPAL libImageIO ${TARGET_LINKER_FLAGS})

install(TARGETS OPAL DESTINATION bin)
