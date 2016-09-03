set (Matrix_SRCS Matrix.h Matrix.cpp)

add_library(libOPAL STATIC ${Matrix_SRCS})

target_include_directories(libOPAL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
