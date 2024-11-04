#COMMON

set( srcs "errors_events.c" "stdioX.c" "terminalX.c" "timeX.c" "utilitiesX.c" )
set( include_dirs "." )
set( priv_include_dirs )
set( requires "printfx rtos-support" )
set( priv_requires )

idf_component_register(
	SRCS ${srcs}
	INCLUDE_DIRS ${include_dirs}
	PRIV_INCLUDE_DIRS ${priv_include_dirs}
	REQUIRES ${requires}
	PRIV_REQUIRES ${priv_requires}
)
