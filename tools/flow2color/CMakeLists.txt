add_executable(flow2color flow2color.cpp
                          main.cpp
                          "${CMAKE_CURRENT_SOURCE_DIR}/../../lib/lodepng/lodepng.cpp")
target_include_directories(flow2color PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../lib")