set (SOURCES fs/File.cpp
             fs/Directory.cpp
             json/json.cpp
             string/Split.cpp)

add_subdirectory(json/json11)

add_library(libUtil STATIC ${SOURCES})

target_link_libraries(libUtil json11)
target_include_directories(libUtil PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
