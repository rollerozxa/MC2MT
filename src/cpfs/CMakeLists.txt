cmake_minimum_required(VERSION 3.5)

project(CPFS)

add_library("${PROJECT_NAME}" "${PROJECT_SOURCE_DIR}/cpfs.cpp")

add_executable("ls" "${PROJECT_SOURCE_DIR}/ls.cpp")
target_link_libraries("ls" ${PROJECT_NAME})

set_target_properties("${PROJECT_NAME}" "ls" PROPERTIES
	COMPILE_FLAGS "-ggdb -O0 -std=c++11 -Wall -Wextra -Wpedantic"
	RUNTIME_OUTPUT_DIRECTORY "bin"
	ARCHIVE_OUTPUT_DIRECTORY "lib")
