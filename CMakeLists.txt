cmake_minimum_required(VERSION 3.18)

project(shttpd)

aux_source_directory(./adt ALL_SRCS_0)
aux_source_directory(. ALL_SRCS_1)
add_executable(shttpd main.cpp ${ALL_SRCS_1} ${ALL_SRCS_0})

target_link_libraries(shttpd pthread)
