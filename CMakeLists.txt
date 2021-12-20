#COMMON
set( srcs "options.c" "x_errors_events.c" "x_stdio.c" "x_terminal.c" "x_time.c" "x_utilities.c" )
set( include_dirs "." )
set( requires rtos-support )
set( priv_requires buffers hal_esp32 http printfx stringsX syslog telnet_server)
idf_component_register(
	SRCS ${srcs} 
	INCLUDE_DIRS ${include_dirs} 
	REQUIRES ${requires} 
	PRIV_REQUIRES ${priv_requires}
)
