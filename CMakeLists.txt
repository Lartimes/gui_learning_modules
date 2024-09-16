cmake_minimum_required(VERSION 3.20)
project(GTK4Learning C)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)

include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${GTK_LIBRARIES})