#!/bin/bash
/usr/bin/tolua++ -L virtual_method_hooks.lua -o Bindings.cpp -H Bindings.h AllToLua.pkg
