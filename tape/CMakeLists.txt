add_library(tape INTERFACE)
target_include_directories(tape INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_sources(tape INTERFACE utils.cpp)

add_subdirectory(tests)