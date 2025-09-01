#COMMON

set( srcs "common-vars.c" "errors_events.c" "stdioX.c" "timeX.c" "utilitiesX.c" )
set( include_dirs "include" )
set( priv_include_dirs )
set( requires "hal_esp32 stringsX" )
set( priv_requires )

idf_component_register(
	SRCS ${srcs}
	INCLUDE_DIRS ${include_dirs}
	PRIV_INCLUDE_DIRS ${priv_include_dirs}
	REQUIRES ${requires}
	PRIV_REQUIRES ${priv_requires}
)
