"tolua++.exe" -L virtual_method_hooks.lua -o Bindings.cpp -H Bindings.h AllToLua.pkg

if %ALLTOLUA_WAIT%N == N pause
