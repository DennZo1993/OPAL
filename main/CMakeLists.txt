set(Main_SRCS main.cpp)

add_executable(OPAL ${Main_SRCS})

if (("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") OR
    ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU"))
  target_compile_options(OPAL PRIVATE -Wall -Wextra -Werror)
endif()

target_include_directories(OPAL PRIVATE ../lib ../lib/ImageIO)
target_link_libraries(OPAL libOPAL libImageIO)

install(TARGETS OPAL DESTINATION bin)
