cmake_minimum_required(VERSION 2.8.4)
project(em400 C)

set(WITH_DEBUGGER 1)

set(DEBUG 0)

include(appversion.cmake)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(Threads REQUIRED)
find_package(emdas REQUIRED)
find_package(emcrk REQUIRED)
find_package(emawp REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_BINARY_DIR}/src)

add_definitions(-Wall -std=c99 -DEM400_VERSION="${APP_VERSION}")

if(DEBUG)
	add_definitions(-DDEBUG -g -O0)
else(DEBUG)
	add_definitions(-DNDEBUG -O3)
endif(DEBUG)

if(WITH_DEBUGGER)
	set(CURSES_NEED_NCURSES TRUE)
	find_package(Curses REQUIRED)
	include_directories(SYSTEM ${CURSES_INCLUDE_DIR})
	include_directories(${CMAKE_BINARY_DIR}/debugger)

	find_path(READLINE_INCLUDE_DIR readline/readline.h)
	find_library(READLINE_LIBRARY NAMES readline)
	include_directories(${READLINE_INCLUDE_DIR})

	add_definitions(-DWITH_DEBUGGER)
	set(DEBUGGER_LIBS debugger ${CURSES_LIBRARIES} ${READLINE_LIBRARY})
else()
	set(DEBUGGER_LIBS)
endif()

# Workaround for QtCreator to see header, lexer and parser files
FILE(GLOB_RECURSE headers "include/*.h")
add_custom_target(headers SOURCES ${headers})
FILE(GLOB_RECURSE parser "src/*.[ly]")
add_custom_target(parser SOURCES ${parser})

add_subdirectory(src)

# vim: tabstop=4
