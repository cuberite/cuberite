
:: AllToLua_Lua.bat
:: This scripts updates the automatically-generates Lua bindings in Bindings.cpp / Bindings.h
:: When called without any parameters, it will pause for a keypress at the end
:: Call with any parameter to disable the wait (for buildserver use)
:: This script assumes "lua" executable to be in PATH, it uses a pure-lua implementation of the ToLua processor

@echo off





:: Regenerate the files:
echo Regenerating LUA bindings . . .
lua ..\..\lib\tolua++\src\bin\lua\_driver.lua -L BindingsProcessor.lua -o Bindings.cpp -H Bindings.h AllToLua.pkg




: Wait for keypress, if no param given:
echo.
if %ALLTOLUA_WAIT%N == N pause




