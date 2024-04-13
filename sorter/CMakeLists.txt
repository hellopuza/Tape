add_library(sorter)
target_include_directories(sorter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(sorter PUBLIC tape)

target_sources(sorter PRIVATE sorter.cpp)

add_subdirectory(tests)
