add_library(sorter INTERFACE)
target_include_directories(sorter INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(sorter INTERFACE tape)

add_subdirectory(tests)