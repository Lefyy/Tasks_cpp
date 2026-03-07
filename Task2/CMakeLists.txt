cmake_minimum_required(VERSION 3.16)
project(Task2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB TASK2_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/domain/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/service/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/repository/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cli/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cli/commands/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.cpp"
)

add_executable(task2 ${TASK2_SOURCES})
target_include_directories(task2 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

