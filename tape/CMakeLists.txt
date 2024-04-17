add_library(tape INTERFACE)
target_include_directories(tape INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)

add_subdirectory(tests)