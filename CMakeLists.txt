cmake_minimum_required(VERSION 3.5)

project(MC2MT)

add_executable(${PROJECT_NAME}
	"${PROJECT_SOURCE_DIR}/src/main.cpp"
	"${PROJECT_SOURCE_DIR}/src/convert.cpp"
	"${PROJECT_SOURCE_DIR}/src/threads.cpp"
	"${PROJECT_SOURCE_DIR}/src/MCMap.cpp"
	"${PROJECT_SOURCE_DIR}/src/MTMap.cpp"
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules/")

add_subdirectory(src/nbt)
add_subdirectory(src/cpfs)

find_package(Threads REQUIRED)
find_package(SQLite3 REQUIRED)

target_link_libraries(${PROJECT_NAME}
	Threads::Threads
	"nbt"
	"CPFS"
	${ZLIB_LIBRARY}
	${SQLite3_LIBRARY}
)
include_directories(
	"${NBT_SOURCE_DIR}"
	"${CPFS_SOURCE_DIR}"
	${ZLIB_INCLUDE_DIRS}
	${SQLite3_INCLUDE_DIR}
)

if (MINGW)
	target_link_libraries(${PROJECT_NAME} ws2_32.lib)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mthreads")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
	COMPILE_FLAGS "-std=c++11 -Wall -Wextra -Wpedantic"
	RUNTIME_OUTPUT_DIRECTORY "bin")
