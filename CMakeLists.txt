#COMMON

set( vBase "/Users/andremaree/Dropbox/Devs/ws/" )
string( LENGTH ${vBase} vBaseLen )

set( vIrmacs "esp/irmacs" )
string( LENGTH ${vIrmacs} vIrmacsLen )

set( vCompTest "osx/ComponentTest" )
string( LENGTH ${vCompTest} vCompTestLen )

set( vNow ${CMAKE_SOURCE_DIR} )
string( SUBSTRING ${vNow} ${vBaseLen} ${vIrmacsLen} vOpt1 )
string( SUBSTRING ${vNow} ${vBaseLen} ${vCompTestLen} vOpt2 )

if( vOpt1 MATCHES esp/irmacs )
	idf_component_register(
		SRCS "x_errors_events.c" "x_stdio.c" "x_terminal.c" "x_time.c" "x_utilities.c"
		INCLUDE_DIRS "."
		REQUIRES hal_esp32 buffers
		PRIV_REQUIRES irmacos printfx rtos-support stringsX values
	)

elseif( vOpt2 MATCHES osx/ComponentTest )
	project(common)
	add_library(common "x_time.c" "x_utilities.c" )
	target_include_directories(common PUBLIC ".")

else()
	message( "vOpt1 : ${vOpt1}" )
	message( "vOpt2 : ${vOpt2}" )
endif()
