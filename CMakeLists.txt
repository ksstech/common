#COMMON

set( psd ${CMAKE_BINARY_DIR} )
string( LENGTH ${psd} len )
math( EXPR len "${len} - 12" )
string( SUBSTRING ${psd} ${len} 6 pn )

if( pn MATCHES irmacs )
	idf_component_register(
		SRCS "x_errors_events.c" "x_stdio.c" "x_terminal.c" "x_time.c" "x_utilities.c"
		INCLUDE_DIRS "."
		REQUIRES hal_esp32 buffers
		PRIV_REQUIRES irmacos printfx rtos-support stringsX values
	)
else()
#	set( REQUIRES hal_esp32 buffers )
#	set( PRIV_REQUIRES irmacos printfx rtos-support stringsX values )
	project(common)
	# "x_errors_events.c" "x_stdio.c" "x_terminal.c" "x_time.c" "x_utilities.c" )
	add_library(common "x_time.c" "x_utilities.c" )
	target_include_directories(common PUBLIC ".")
endif()
