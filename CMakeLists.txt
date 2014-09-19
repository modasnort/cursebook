cmake_minimum_required (VERSION 2.6)
project (cursebook)
# SET(CMAKE_MODULE_PATH ".")

# SET( CMAKE_C_FLAGS  " ${CMAKE_C_FLAGS} " )

add_definitions(-std=c99)

include_directories("./src")
SET(EXECUTABLE_OUTPUT_PATH ".")

add_executable(cursebook

	ui.c
	thread.c

	main.c
)

SET( LIBRARIES "-lncurses -lpthread" )

target_link_libraries(cursebook "${LIBRARIES}" )

