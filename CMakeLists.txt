cmake_minimum_required(VERSION 3.5)

project(MC2MT)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

message(STATUS "*** Building MC2MT ***")

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules/")

find_package(SQLite3 REQUIRED)
find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(
	${SQLite3_INCLUDE_DIR}
	${ZLIB_INCLUDE_DIRS})

set(SRCS
	src/main.cpp
	src/convert.cpp
	src/threads.cpp
	src/MCMap.cpp
	src/MTMap.cpp
	src/nbt/nbt.cpp
	src/nbt/serialization.cpp
	src/nbt/compression.cpp)

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME}
	${SQLite3_LIBRARY}
	Threads::Threads
	${ZLIB_LIBRARY})


if (MINGW)
	target_link_libraries(${PROJECT_NAME} ws2_32.lib)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mthreads")
endif()

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/bin")
