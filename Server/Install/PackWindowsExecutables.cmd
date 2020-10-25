@echo off
:: This script is run in the Appveyor CI builds to pack up the artifacts
:: It is expected to be run with the Server folder as the current working dir
@echo on

:: Only test that the server runs for pull requests
:: Don't upload any artifacts until it's merged into master
if defined APPVEYOR_PULL_REQUEST_NUMBER (
	echo stop  1>>cmds.txt
	Cuberite --port 32767  0<cmds.txt
	exit /b
)

:: Main executables
echo Cuberite %APPVEYOR_JOB_NAME%-#%APPVEYOR_BUILD_NUMBER%  1>buildinfo.txt
7z a -tzip -y Cuberite.zip -scsWIN -i@Install\WindowsExecutables.list -xr!*.git*
7z a -tzip -y PDBs.zip -scsWIN -i@Install/WindowsPDBs.list -xr!*.git*

:: Generate API documentation
git clone https://github.com/madmaxoft/ManualApiDump Plugins/ManualApiDump
echo load ManualApiDump  1>cmds.txt
echo manualapi  1>>cmds.txt
echo load APIDump  1>>cmds.txt
echo api  1>>cmds.txt
echo stop  1>>cmds.txt
Cuberite --port 32767  0<cmds.txt

:: API documentation
7z a -tzip -y ManualAPI.zip -scsWIN "ManualAPI.lua"
7z a -tzip -y AutoAPI.zip -scsWIN ".\BindingsDocs\*.lua" -x!_raw.lua

:: Upload artifacts
appveyor PushArtifact Cuberite.zip
appveyor PushArtifact PDBs.zip
appveyor PushArtifact AutoAPI.zip
appveyor PushArtifact ManualAPI.zip
appveyor PushArtifact .luacheckrc
