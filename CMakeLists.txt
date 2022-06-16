cmake_minimum_required(VERSION 3.18)

// 项目名称
project(shttpd)

// 添加源文件目录，不用添加头文件，并将源文件保存到变量 ALL_SRCS
aux_source_directory(./src ALL_SRCS)
// 添加生成目标
add_executable(shttpd main.cpp ${ALL_SRCS})

// 添加链接库
target_link_libraries(shttpd pthread)
