set (SOURCES RGBAPixel.cpp
             OPAL.cpp
             OPALSettings.cpp

             util/fs/File.cpp
             util/fs/Directory.cpp
             util/string/Split.cpp)

add_subdirectory(ImageIO)
add_subdirectory(json11)

add_library(libOPAL STATIC ${SOURCES})

set(LINK_FLAGS "")

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

  if ("${CMAKE_BUILD_TYPE}" MATCHES "DEBUG")
    message("Debug build with coverage support")
    target_compile_options(libOPAL PRIVATE
                           -g -O0
                           -fprofile-arcs
                           -ftest-coverage)
    set(LINK_FLAGS ${LINK_FLAGS} -lgcov)
  endif()

  set(LINK_FLAGS ${LINK_FLAGS} -lstdc++fs)
endif()

target_link_libraries(libOPAL libImageIO json11 ${LINK_FLAGS})

target_include_directories(libOPAL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
