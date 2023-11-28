cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(rapiplay)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." FALSE)
option(ENABLE_TESTING "Enable testing" TRUE)

include("${CMAKE_SOURCE_DIR}/cmake/CompileOptions.cmake")

find_package(StbImage)

set(HEADERS
	Error.h
	Point.h
	Color.h
	MemBlock.h
	FileDescriptor.h
	FrameBuffer.h
	StbTexture.h
	Utils.h
	flying_line_demo.h
)

set(SOURCES
	MemBlock.cpp
	FileDescriptor.cpp
	FrameBuffer.cpp
	StbTexture.cpp
	flying_line_demo.cpp
	StbiImpl.cpp
	Utils.cpp
	StbiResizeImpl.cpp
	StbiImageWriteImpl.cpp
)

add_library(rapi ${HEADERS} ${SOURCES})
target_include_directories(rapi PUBLIC ${STB_IMAGE_INCLUDE_DIR})

add_executable(rapishow rapishow.cpp)
target_link_libraries(rapishow PRIVATE rapi)

add_executable(rapiplay rapiplay.cpp)
target_link_libraries(rapiplay PRIVATE rapi)

add_executable(rapiresize rapiresize.cpp)
target_link_libraries(rapiresize PRIVATE rapi)
