cmake_minimum_required (VERSION 2.6)
project (cursebook)

add_definitions( -std=c99 -Werror )

# GNU
add_definitions( -D_XOPEN_SOURCE=500 )
######

# debug
add_definitions( "-O0 -s -g3" )
######

include_directories("./src")
SET(EXECUTABLE_OUTPUT_PATH ".")

add_executable(cursebook

	pi.c
	types.c
	ui.c
	thread.c
	input.c
	console.c
	fb.c

	main.c
)

SET( LIBRARIES "-lncurses -lpthread" )

target_link_libraries(cursebook "${LIBRARIES}" )

