find_package(Threads REQUIRED)

include(AddFlagIfSupported)
include(GetCoverageFlags)

# Enable ExternalProject CMake module
include(ExternalProject)

if (MSVC)
  set(LibSuffix ".lib")
else()
  set(LibSuffix ".a")
endif()

# Download and install GoogleTest
ExternalProject_Add(
    gtest
    URL http://pkgs.fedoraproject.org/repo/pkgs/gtest/gtest-1.7.0.zip/2d6ec8ccdf5c46b05ba54a9fd1d130d7/gtest-1.7.0.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=
               -Dgtest_force_shared_crt=ON
    # Disable install step
    INSTALL_COMMAND ""
)

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

# Set gtest properties
ExternalProject_Get_Property(gtest source_dir binary_dir)
set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${LibSuffix}"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}")
# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/include")

# Download and install GoogleMock
ExternalProject_Add(
    gmock
    URL http://pkgs.fedoraproject.org/repo/pkgs/gmock/gmock-1.7.0.zip/073b984d8798ea1594f5e44d85b20d66/gmock-1.7.0.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gmock
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=
               -Dgtest_force_shared_crt=ON
               -Dgmock_force_shared_crt=ON
    # Disable install step
    INSTALL_COMMAND ""
)

# Create a libgmock target to be used as a dependency by test programs
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gmock gtest)

# Set gmock properties
ExternalProject_Get_Property(gmock source_dir binary_dir)
set_target_properties(libgmock PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gmock${LibSuffix}"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
#    "INTERFACE_INCLUDE_DIRECTORIES" "${source_dir}/include"
)
# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/include")

# Testing project
SET(TEST_SOURCES main.cpp
                 Common.cpp

                 Image/Constructors.cpp
                 Image/Allocation.cpp
                 Image/Misc.cpp
                 Image/AdditionSubtraction.cpp
                 Image/Relation.cpp
                 Image/Assign.cpp
                 Image/StreamIO.cpp

                 ImageDatabaseTests.cpp

                 OPAL/Constructor.cpp
                 OPAL/Initialization.cpp
                 OPAL/MaxVoteLabelEstimatorTest.cpp
                 OPAL/SSD.cpp

                 ImageIO/AnalyzeImageReaderTest.cpp
                 ImageIO/PngImageReaderTest.cpp

                 util/FileTest.cpp
                 util/DirectoryTest.cpp
                 util/JsonTest.cpp
    )

# Compiler flags.
add_flag_if_supported("-std=c++11"  TARGET_COMPILER_FLAGS)

# Linker flags.
set(TARGET_LINKER_FLAGS "")


ADD_EXECUTABLE(Tests ${TEST_SOURCES})

target_compile_options(Tests PRIVATE ${TARGET_COMPILER_FLAGS})

if ("${CMAKE_BUILD_TYPE}" MATCHES "Coverage")
  get_coverage_flags(COVERAGE_COMPILER_FLAGS)
  target_compile_options(Tests PRIVATE ${COVERAGE_COMPILER_FLAGS})

  list(APPEND TARGET_LINKER_FLAGS "-lgcov" "--coverage")
endif()

TARGET_LINK_LIBRARIES(Tests
    libOPAL
    libgtest
    libgmock
    ${TARGET_LINKER_FLAGS}
)

add_custom_command(TARGET Tests PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_CURRENT_SOURCE_DIR}/test_data ${CMAKE_CURRENT_BINARY_DIR}/test_data)

add_test(NAME Tests COMMAND Tests)
