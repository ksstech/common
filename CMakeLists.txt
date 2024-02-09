#COMMON

set( srcs "x_errors_events.c" "x_stdio.c" "x_terminal.c" "x_time.c" "x_utilities.c" )
set( include_dirs "." )
set( priv_include_dirs )
idf_build_get_property( EXTRA_REQUIRES EXTRA_REQUIRES )
set( requires ${EXTRA_REQUIRES})
set( priv_requires "buffers http stringsX telnet_server printfx syslog systiming" )

idf_component_register(
	SRCS ${srcs}
	INCLUDE_DIRS ${include_dirs}
	PRIV_INCLUDE_DIRS ${priv_include_dirs}
	REQUIRES ${requires}
	PRIV_REQUIRES ${priv_requires}
)
