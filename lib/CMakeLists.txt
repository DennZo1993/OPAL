set (SOURCES util.cpp
             RGBAPixel.cpp
             OPAL.cpp)

add_subdirectory(ImageIO)
add_subdirectory(json11)

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

  target_link_libraries(libOPAL libImageIO "-lstdc++fs")
else()
  target_link_libraries(libOPAL libImageIO)
endif()

target_include_directories(libOPAL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
