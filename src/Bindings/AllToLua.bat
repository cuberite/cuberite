
:: AllToLua.bat
:: This scripts updates the automatically-generates Lua bindings in Bindings.cpp / Bindings.h
:: When called without any parameters, it will pause for a keypress at the end
:: Call with any parameter to disable the wait (for buildserver use)

@echo off





:: Regenerate the files:
echo Regenerating LUA bindings . . .
"tolua++.exe" -L BindingsProcessor.lua -o Bindings.cpp -H Bindings.h AllToLua.pkg




: Wait for keypress, if no param given:
echo.
if %ALLTOLUA_WAIT%N == N pause




