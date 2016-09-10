set(Main_SRCS main.cpp)

add_executable(OPAL ${Main_SRCS})

if (("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") OR
    ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU"))
  target_compile_options(OPAL PRIVATE -Wall -Wextra -Werror)
endif()


target_link_libraries(OPAL libOPAL)

install(TARGETS OPAL DESTINATION bin)
