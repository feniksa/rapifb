cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

project(rapiplay VERSION 1.0.0 
	DESCRIPTION "Linux framebuffer tools" 
	HOMEPAGE_URL "https://github.com/feniksa/rapifb"
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." FALSE)
option(ENABLE_TESTING "Enable testing" TRUE)

include("${CMAKE_SOURCE_DIR}/cmake/CompileOptions.cmake")

find_package(Boost 1.74 REQUIRED COMPONENTS program_options log)
find_package(StbImage REQUIRED)

add_subdirectory(fb)
add_subdirectory(tools)

set(CPACK_DEBIAN_PACKAGE_DEPENDS
	"libboost-program-options1.74.0,libboost-log1.74.0")
set(CPACK_PACKAGE_CONTACT "Maksym Sditanov <feniksa@200volts.com>")

include(CPack)
