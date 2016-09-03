set(Main_SRCS main.cpp)

add_executable(OPAL ${Main_SRCS})

target_link_libraries(OPAL libOPAL)

install(TARGETS OPAL DESTINATION bin)
