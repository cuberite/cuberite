
:: AllToLua.bat

:: This scripts updates the automatically-generates Lua bindings in Bindings.cpp / Bindings.h





:: If there was a Git conflict, resolve it by resetting to HEAD; we're regenerating the files from scratch anyway
git checkout -- Bindings.cpp
git checkout -- Bindings.h





:: Regenerate the files:
"tolua++.exe" -L virtual_method_hooks.lua -o Bindings.cpp -H Bindings.h AllToLua.pkg





if %ALLTOLUA_WAIT%N == N pause
