
:: AllToLua.bat
:: This scripts updates the automatically-generates Lua bindings in Bindings.cpp / Bindings.h
:: When called without any parameters, it will pause for a keypress at the end
:: Call with any parameter to disable the wait (for buildserver use)

@echo off





:: Regenerate the files:
echo Regenerating LUA bindings...
"tolua++.exe" -L virtual_method_hooks.lua -o Bindings.cpp -H Bindings.h AllToLua.pkg >nul




: Wait for keypress, if no param given:
echo Bindings were generated
echo.
echo Please depress the 'any' key to continue
if %ALLTOLUA_WAIT%N == N pause >nul




