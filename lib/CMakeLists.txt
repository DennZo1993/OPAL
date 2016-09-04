set (Matrix_SRCS Matrix.h Matrix.cpp)
set (ImageDatabase_SRCS ImageDatabase.h ImageDatabase.cpp)

add_library(libOPAL STATIC ${Matrix_SRCS} ${ImageDatabase_SRCS})

target_include_directories(libOPAL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})