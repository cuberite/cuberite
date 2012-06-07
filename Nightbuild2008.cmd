@echo off
:: Nightbbuild2008.cmd
:: This script is run every night to produce a new version of MCServer, backup its PDB files and upload the packages to web.
:: These sub-scripts are used:
::  - WCRev.cmd together with subwcrev templating to obtain the version number as an environment var
::  - UploadVersion.ftp FTP command template for uploading the version to the web (not included in the SVN, because it contains confidential passwords! Use your own :)
:: When run without parameters, this script pauses at the end and waits for a keypress.
:: To run in an automated scheduler, add any parameter to disable waiting for a keystroke
::
:: This script expects a few tools on specific paths, you can pass the correct paths for your system as env vars "zip", "vc" and "tsvn"


:: 7-zip executable (by default it should be on PATH):
if %zip%a == a set zip=7z

:: Visual C++ compiler executable name:
if %vc%a == a set vc="vcbuild.exe"

:: TortoiseProc executable (for updating to the latest version):
if %tsvn%a== a set tsvn="c:\program files\tortoisesvn\bin\tortoiseproc.exe"

:: Subwcrev (from TortoiseSVN, for querying revision number; by default in PATH):
if %subwcrev%a == a set subwcrev=subwcrev




echo Performing nightbuild of MC-Server


set DONOTPAUSE=y

:: Update the sources to the latest revision:
start "tsvn src" /b /min /wait %tsvn% /command:update /path:. /closeonend:1
if errorlevel 1 goto haderror


:: Copy all *.template files into their non-template versions, substituting SVN keywords:
for /r %%X in (*.template) do (
	%subwcrev% . "%%X" "%%~dpX%%~nX"
	if errorlevel 1 goto haderror
)


:: Get the revision number into WCREV env var
call Install\WCVersion.cmd
echo WCREV = %WCREV%
 

:: Test if the version is already present
if exist MCServer_Win_%WCREV%.7z (
	echo Latest version already present, bailing out
	goto end
)



:: Update Bindings.cpp
echo Updating Lua bindings
set ALLTOLUA_WAIT=N
cd source
call AllToLua.bat
cd ..




:: Compile using VC2008 Express. Do a full rebuild.
echo Setting up VS environment...
call "%VS90COMNTOOLS%\vsvars32.bat"
echo Compiling MCServer...
title MCS Nightbuild
start "vc" /b /wait /low /min %vc% /r vc2008\MCServer.sln "Release|Win32"
if errorlevel 1 goto haderror




:: Use 7-zip to compress the resulting files into a single file:
:: Note: the output filename here must be the same as in the upload_win.ftp.template script
copy VC2008\Release\MCServer.exe MCServer.exe
%zip% a -mx9 -y MCServer_Win_%WCREV%.7z -scsWIN @Install\Zip2008.list
if errorlevel 1 goto haderror

:: Also pack PDBs into a separate archive:
:: Note: the output filename here must be the same as in the upload_win.ftp.template script
%zip% a -mx9 -y MCServer_Win_%WCREV%_PDBs.7z -scsWIN @Install\Zip2008_PDBs.list
if errorlevel 1 goto haderror





:: upload to FTP using the upload_win.ftp (.template) script
ftp -n -s:Install\upload_win.ftp xoft.cz




goto end




:haderror
echo an error was encountered, check command output above
pause
goto finished





:end
if "a%1" == "a" pause



:finished