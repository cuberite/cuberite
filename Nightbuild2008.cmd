@echo off
:: Nightbbuild2008.cmd
:: This script is run every night to produce a new version of MCServer, backup its PDB files and upload the packages to web.
:: These sub-scripts are used:
::  - UploadVersion.ftp FTP command template for uploading the version to the web
:: When run without parameters, this script pauses at the end and waits for a keypress.
:: To run in an automated scheduler, add any parameter to disable waiting for a keystroke
::
:: This script expects a few tools on specific paths, you can pass the correct paths for your system as env vars "zip" and "vc"
:: This script assumes that "git", "symstore" and "touch" are available on PATH.
:: git comes from msysgit
:: symstore comes from Microsoft's Debugging Tools for Windows
:: touch comes from unxtools
:: This script is locale-dependent


:: 7-zip executable (by default it should be on PATH):
if %zip%a == a set zip=7z

:: Visual C++ compiler executable name:
if %vc%a == a set vc="vcbuild.exe"




:: Get the date and time into vars:
For /f "tokens=2-4 delims=/. " %%a in ('date /t') do (
	set MYYEAR=%%c
	set MYMONTH=%%b
	set MYDAY=%%a
)
For /f "tokens=1-2 delims=/:" %%a in ('time /t') do (set MYTIME=%%a_%%b)

echo MYYEAR = %MYYEAR%
echo MYMONTH = %MYMONTH%
echo MYDAY = %MYDAY%
echo MYTIME = %MYTIME%

echo Performing nightbuild of MC-Server





set DONOTPAUSE=y

:: Update the sources to the latest revision:
del source\Bindings.cpp
del source\Bindings.h
git checkout -- source\Bindings.*
git pull
if errorlevel 1 goto haderror



:: Get the Git commit ID into an environment var
For /f "tokens=1 delims=/. " %%a in ('git log -1 --oneline --no-abbrev-commit') do (set COMMITID=%%a)
if errorlevel 1 goto haderror



:: Test if the version is already present, using a "tagfile" that we create upon successful build
set TAGFILE=Install\built_%COMMITID%.tag
if exist %TAGFILE% (
	echo Latest version already present, bailing out
	goto end
)



:: Update the Bindings:
del source\Bindings.cpp
del source\Bindings.h
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





:: Copy all the example ini files into the Install folder for zipping:
copy MCServer\*.example.ini Install\*.example.ini

:: Use 7-zip to compress the resulting files into a single file:
set FILESUFFIX=%MYYEAR%_%MYMONTH%_%MYDAY%_%MYTIME%_%COMMITID%
echo FILESUFFIX=%FILESUFFIX%
copy MCServer\MCServer.exe Install\MCServer.exe
cd Install
%zip% a -mx9 -y MCServer_Win_%FILESUFFIX%.7z -scsWIN @Zip2008.list
if errorlevel 1 goto haderror
cd ..

:: Also pack PDBs into a separate archive:
%zip% a -mx9 -y Install\PDBs_%FILESUFFIX%.7z -scsWIN @Install\Zip2008_PDBs.list
if errorlevel 1 goto haderror





:: upload to the FTP:
:upload
if "a%ftppass%" == "a" (
	echo You need to set FTP password in the ftppass environment variable to upload the files
	goto end
)
if "a%ftpuser%" == "a" (
	echo You need to set FTP username in the ftpuser environment variable to upload the files
	goto end
)
if "a%ftpsite%" == "a" (
	echo You need to set FTP server in the ftpsite environment variable to upload the files
	goto end
)
ncftpput -p %ftppass% -u %ftpuser% -T temp_ %ftpsite% / Install\MCServer_Win_%FILESUFFIX%.7z
if errorlevel 1 goto haderror
ncftpput -p %ftppass% -u %ftpuser% -T temp_ %ftpsite% /PDBs Install\PDBs_%FILESUFFIX%.7z
if errorlevel 1 goto haderror
echo Upload finished.




:: Create the tagfile so that we know that this CommitID has been built already
touch %TAGFILE%





:: Add the symbols to a global symbol cache
:: We want per-month symbol caches, so that the old ones can be easily deleted
set SYMBOLS=Symbols\%MYYEAR%_%MYMONTH%\
echo Storing symbols in %SYMBOLS%

symstore add /f MCServer\MCServer.* /s %SYMBOLS% /t MCServer
if errorlevel 1 goto haderror



goto end




:haderror
echo an error was encountered, check command output above
pause
goto finished





:end
if "a%1" == "a" pause



:finished