cmake_minimum_required(VERSION 3.20)

project(TapeSorter VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Werror -Wshadow")

find_package(GTest REQUIRED)

function(add_test_module MODULE_NAME)
    add_library(${MODULE_NAME}_test)
    target_link_libraries(${MODULE_NAME}_test PUBLIC
        ${GTEST_LIBRARIES}
        ${MODULE_NAME}
    )

    target_sources(${MODULE_NAME}_test PUBLIC test_${MODULE_NAME}.cpp)
    target_link_libraries(test_all PRIVATE ${MODULE_NAME}_test)

    add_executable(test_${MODULE_NAME})
    target_link_libraries(test_${MODULE_NAME} PRIVATE ${MODULE_NAME}_test)
    target_sources(test_${MODULE_NAME} PRIVATE main.cpp test_${MODULE_NAME}.cpp)
endfunction(add_test_module)

add_executable(test_all)

add_subdirectory(tape)
add_subdirectory(sorter)
add_subdirectory(tests)

add_executable(tape_sorter app.cpp)
target_link_libraries(tape_sorter PRIVATE tape sorter)
