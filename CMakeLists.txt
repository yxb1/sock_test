cmake_minimum_required(VERSION 3.0)
project(sock_test)

message(STATUS "Enter cmake ${CMAKE_CURRENT_LIST_DIR}")

file(GLOB_RECURSE CODE_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE CODE_INCS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
message(STATUS "Source files ${CODE_SRCS}")
message(STATUS "Header files ${CODE_INCS}")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/bin)

add_executable(${PROJECT_NAME} ${CODE_SRCS})
target_link_libraries(${PROJECT_NAME} pthread)