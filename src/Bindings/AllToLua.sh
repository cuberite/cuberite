#!/bin/bash
/usr/bin/tolua++ -L BindingsProcessor.lua -o Bindings.cpp -H Bindings.h AllToLua.pkg
