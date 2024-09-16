cmake_minimum_required(VERSION 3.20)
project(GTK4Learning C)
#https://discourse.gnome.org/t/help-with-a-cmakelists-txt-for-gtk-4/7397
#gtk4
set(CMAKE_C_STANDARD 11)
# 设置源文件目录
set(SOURCE_DIR "src")

# 使用GLOB_RECURSE命令递归地扫描src目录下的所有文件
file(GLOB_RECURSE ALL_FILES "${SOURCE_DIR}/*")

# 初始化源文件列表和头文件列表
set(SOURCES "")
set(HEADERS "")

# 遍历所有文件，将C文件添加到源文件列表，将头文件添加到头文件列表
foreach(FILE IN LISTS ALL_FILES)
    get_filename_component(FILE_EXT "${FILE}" EXT)
    if(FILE_EXT STREQUAL ".c")
        list(APPEND SOURCES "${FILE}")
    elseif(FILE_EXT STREQUAL ".h" OR FILE_EXT STREQUAL ".hpp")
        list(APPEND HEADERS "${FILE}")
    endif()
endforeach()


# 添加可执行文件
add_executable(GTK4Learning ${SOURCES} ${ALL_FILES})

# 设置包含目录
target_include_directories(GTK4Learning PRIVATE ${SOURCE_DIR})

# 查找并链接GTK4库
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)
target_link_libraries(GTK4Learning ${GTK4_LIBRARIES})
target_include_directories(GTK4Learning PRIVATE ${GTK4_INCLUDE_DIRS})
include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})