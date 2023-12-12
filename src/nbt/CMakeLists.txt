cmake_minimum_required(VERSION 3.5)

project(NBT)
set(PROJECT_NAME_LOWER "nbt")

add_library("${PROJECT_NAME_LOWER}" STATIC
	"${PROJECT_SOURCE_DIR}/nbt.cpp"
	"${PROJECT_SOURCE_DIR}/serialization.cpp"
	"${PROJECT_SOURCE_DIR}/compression.cpp"
)

add_executable("${PROJECT_NAME_LOWER}-test"
	"${PROJECT_SOURCE_DIR}/test.cpp"
)

find_package(ZLIB REQUIRED)

target_link_libraries("${PROJECT_NAME_LOWER}" ${ZLIB_LIBRARY})
target_link_libraries("${PROJECT_NAME_LOWER}-test" "${PROJECT_NAME_LOWER}" ${ZLIB_LIBRARY})
include_directories(${ZLIB_INCLUDE_DIRS})

set_target_properties("${PROJECT_NAME_LOWER}" "${PROJECT_NAME_LOWER}-test" PROPERTIES
	COMPILE_FLAGS "-std=c++11 -Wall -Wextra -Wpedantic"
	RUNTIME_OUTPUT_DIRECTORY "bin"
	ARCHIVE_OUTPUT_DIRECTORY "bin")

# Only build the tests by default if this is the top-level project
if (NOT "${PROJECT_NAME}" STREQUAL "${CMAKE_PROJECT_NAME}")
	set_target_properties("${PROJECT_NAME_LOWER}-test" PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()
