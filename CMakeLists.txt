#COMMON

set( srcs "options.c" "x_errors_events.c" "x_stdio.c" "x_terminal.c" "x_time.c" "x_utilities.c" )
set( include_dirs "." )
set( priv_include_dirs )
set( requires )
set( priv_requires 
	buffers 
	hal_esp32 
	http 
	printfx
	stringsX 
	syslog
	telnet_server
	rtos-support
)

idf_component_register(
	SRCS ${srcs} 
	INCLUDE_DIRS ${include_dirs} 
	PRIV_INCLUDE_DIRS ${priv_include_dirs}
	REQUIRES ${requires} 
	PRIV_REQUIRES ${priv_requires}
)
