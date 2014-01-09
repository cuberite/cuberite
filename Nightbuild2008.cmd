@echo off
:: Nightbbuild2008.cmd
:: This script is run every night to produce a new version of MCServer, backup its PDB files and upload the packages to web.
:: When run without parameters, this script pauses at the end and waits for a keypress.
:: To run in an automated scheduler, add any parameter to disable waiting for a keystroke
::
:: The sript creates a symbol store (a database of PDB files) that can be used as a single entry in MSVC's symbol path,
:: then any executable / crashdump built by this script can be debugged and its symbols will be found automatically by MSVC,
:: without the users needing to specify the build version or anything.
:: In order to support pruning the symstore, a per-month store is created, so that old months can be removed when no longer needed.
::
:: This script expects a few tools on specific paths, you can pass the correct paths for your system as env vars "zip" and "vc"
:: This script assumes that "git", "symstore" and "touch" are available on PATH.
:: git comes from msysgit
:: symstore comes from Microsoft's Debugging Tools for Windows
:: touch comes from unxtools
:: This script is locale-dependent, because it parses the output of "time" and "date" shell commands


:: 7-zip executable (by default it should be on PATH):
if %zip%a == a set zip=7z

:: Visual C++ compiler executable name:
if %vc%a == a set vc="vcbuild.exe"




:: Check that the required environment vars are available:
if "a%ftppass%" == "a" (
	echo You need to set FTP password in the ftppass environment variable to upload the files
	goto haderror
)
if "a%ftpuser%" == "a" (
	echo You need to set FTP username in the ftpuser environment variable to upload the files
	goto haderror
)
if "a%ftpsite%" == "a" (
	echo You need to set FTP server in the ftpsite environment variable to upload the files
	goto haderror
)




:: Get the date and time into vars:
:: This is locale-dependent!
For /f "tokens=2-4 delims=/. " %%a in ('date /t') do (
	set MYYEAR=%%c
	set MYMONTH=%%b
	set MYDAY=%%a
)
For /f "tokens=1-2 delims=/:" %%a in ('time /t') do (set MYTIME=%%a_%%b)

echo Performing nightbuild of MC-Server





set DONOTPAUSE=y

:: Update the sources to the latest revision:
git pull
if errorlevel 1 goto haderror



:: Update the external plugins to the latest revision:
git submodule update
if errorlevel 1 goto haderror




:: Get the Git commit ID into an environment var
For /f "tokens=1 delims=/. " %%a in ('git log -1 --oneline --no-abbrev-commit') do (set COMMITID=%%a)
if errorlevel 1 goto haderror



:: Test if the version is already present, using a "tagfile" that we create upon successful build
set TAGFOLDER=Install\%MYYEAR%_%MYMONTH%\
set TAGFILE=%TAGFOLDER%built_%COMMITID%.tag
echo Tag file: %TAGFILE%
if exist %TAGFILE% (
	echo Latest version already present, bailing out
	goto end
)



:: Update the Bindings:
echo Updating Lua bindings
del src\Bindings\Bindings.cpp
del src\Bindings\Bindings.h
set ALLTOLUA_WAIT=N
cd src\Bindings
call AllToLua.bat
cd ..\..




:: Compile using VC2008 Express. Do a full rebuild.
echo Setting up VS environment...
call "%VS90COMNTOOLS%\vsvars32.bat"
echo Compiling MCServer...
title MCS Nightbuild
start "vc" /b /wait /low /min %vc% /r vc2008\MCServer.sln "Release|Win32"
if errorlevel 1 goto haderror




:: Generate the .example.ini files by running the server without any ini files:
cd MCServer
del groups.ini
del settings.ini
del webadmin.ini
echo stop | MCServer
cd ..



:: Copy all the example ini files into the Install folder for zipping:
copy MCServer\groups.ini Install\groups.example.ini
copy MCServer\settings.ini Install\settings.example.ini
copy MCServer\webadmin.ini Install\webadmin.example.ini




:: Use 7-zip to compress the resulting files into a single file:
set FILESUFFIX=%MYYEAR%_%MYMONTH%_%MYDAY%_%MYTIME%_%COMMITID%
echo FILESUFFIX=%FILESUFFIX%
copy MCServer\MCServer.exe Install\MCServer.exe
cd Install
%zip% a -mx9 -y MCServer_Win_%FILESUFFIX%.7z -scsWIN -i@Zip2008.list -xr!*.git*
if errorlevel 1 goto haderror
cd ..

:: Also pack PDBs into a separate archive:
%zip% a -mx9 -y Install\PDBs_%FILESUFFIX%.7z -scsWIN @Install\Zip2008_PDBs.list
if errorlevel 1 goto haderror





:: upload to the FTP:
:upload
ncftpput -p %ftppass% -u %ftpuser% -T temp_ %ftpsite% / Install\MCServer_Win_%FILESUFFIX%.7z
if errorlevel 1 goto haderror
ncftpput -p %ftppass% -u %ftpuser% -T temp_ %ftpsite% /PDBs Install\PDBs_%FILESUFFIX%.7z
if errorlevel 1 goto haderror
echo Upload finished.




:: Create the tagfile so that we know that this CommitID has been built already
mkdir %TAGFOLDER%
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