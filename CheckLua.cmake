# CheckLua.cmake

# Checks whether the Lua standalone interpreter is installed on the host system
# If found, sets HAS_LUA_INTERPRETER to 1 and LUA_INTERPRETER_VERSION to the version reported ("5.1" etc.)
# If not found, unsets HAS_LUA_INTERPRETER




execute_process(
	COMMAND lua -e "io.stdout:write(string.match(_VERSION, '%d+%.%d+'))"
	RESULT_VARIABLE LUA_EXECUTED
	OUTPUT_VARIABLE LUA_INTERPRETER_VERSION
)

if ("${LUA_EXECUTED}" STREQUAL "0")
	set(HAS_LUA_INTERPRETER 1)
else()
	unset(HAS_LUA_INTERPRETER)
	unset(LUA_INTERPRETER_VERSION)
endif()

unset(LUA_EXECUTED)
