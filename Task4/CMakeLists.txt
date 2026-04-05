cmake_minimum_required(VERSION 3.16)
project(Task4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.11.0/mingw_64/lib/cmake/Qt6")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(TASK4_SOURCES
    src/main.cpp
    src/model/graph.cpp
    src/model/graph_io.cpp
    src/core/prim_spanning_tree.cpp
    src/logging/app_logger.cpp
    src/controller/graph_controller.cpp
    src/view/main_window.cpp
)

set(TASK4_HEADERS
    headers/model/graph.h
    headers/model/graph_io.h
    headers/core/prim_spanning_tree.h
    headers/logging/app_logger.h
    headers/controller/graph_controller.h
    headers/view/main_window.h
)

add_executable(task4
    ${TASK4_SOURCES}
    ${TASK4_HEADERS}
)

target_include_directories(task4 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

target_link_libraries(task4 PRIVATE Qt6::Widgets)
