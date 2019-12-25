@echo off
:: This script is run in the Appveyor CI builds to pack up the artifacts
:: It is expected to be run with the Server folder as the current working dir
@echo on

del Cuberite.zip
del PDBs.zip
del ManualAPI.zip
del AutoAPI.zip
rd /q /s Plugins\ManualApiDump
7z a -tzip -y Cuberite.zip -scsWIN -i@Install\WindowsExecutables.list -xr!*.git*
7z a -tzip -y PDBs.zip -scsWIN -i@Install/WindowsPDBs.list -xr!*.git*
git clone https://github.com/madmaxoft/ManualApiDump Plugins/ManualApiDump
echo load ManualApiDump  1>cmds.txt
echo manualapi  1>>cmds.txt
echo load APIDump  1>>cmds.txt
echo api  1>>cmds.txt
echo stop  1>>cmds.txt
Cuberite --port 32767  0<cmds.txt
7z a -tzip -y ManualAPI.zip -scsWIN "ManualAPI.lua"
7z a -tzip -y AutoAPI.zip -scsWIN "BindingsDocs\*.lua" -x!_raw.lua
