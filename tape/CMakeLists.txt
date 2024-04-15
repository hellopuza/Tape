add_library(tape)
target_include_directories(tape PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(tape PRIVATE tape.cpp)

add_subdirectory(tests)
