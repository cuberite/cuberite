@echo off
::
:: Profiling using a MSVC standalone profiler
::
:: See http://www.codeproject.com/Articles/144643/Profiling-of-C-Applications-in-Visual-Studio-for-F for details
:: Note that this script assumes a newer version of the tool, Performance Tools 2010, so that it is Win7-compatible
:: Download at http://www.microsoft.com/en-ca/download/details.aspx?id=23205
::
:: This script sets the profiler up, then starts the app, waits for it to finish and stops the profiler.
:: Then it generates all the CSV report files
::
:: It expects the MS Performance tools installed in C:\Program Files\Microsoft Visual Studio 9.0\Team Tools\Performance Tools
:: You can override this path by setting the pt environment variable prior to launching this script
::
:: By default it will launch the release version of MCServer; set the app environment variable to another executable to run that instead.
:: Note that the app needs to be compiled with the "/PROFILE" flag in order for the profiling to work




setlocal

:: Get the 32-bit program files folder
:: Note that in Win7 the %ProgramFiles% variable points to 32-bit program files for normal users and to 64-bit program files for elevated users
:: This script needs elevation, but the performance tools are 32-bit only
if "%programfiles(x86)%p" == "p" (
	set ProgFiles="%ProgramFiles%"
) else (
	set ProgFiles="%ProgramFiles(x86)%"
)

:: Check that the required variables exist, provide defaults:
if %pt%n == n (
	set pt="%ProgFiles:"=%\Microsoft Visual Studio 10.0\Team Tools\Performance Tools"
)

if %app%n == n (
	set app=MCServer_profile.exe
)

if %outputdir%n == n (
	set outputdir=..\Profiling
)




::Create the output directory, if it didn't exist
mkdir %outputdir%





:: Start the profiler
set outputname=profile.vsp
set output=%outputdir%\%outputname%
%pt%\vsperfcmd /start:sample /output:%output%
if errorlevel 1 goto haderror

:: Launch the application via the profiler
%pt%\vsperfcmd /launch:%app%
if errorlevel 1 goto haderror

:: Shut down the profiler (this command waits, until the application is terminated)
%pt%\vsperfcmd /shutdown
if errorlevel 1 goto haderror





:: cd to outputdir, so that the reports are generated there
cd %outputdir%

:: generate the report files (.csv)
%pt%\vsperfreport /summary:all %outputname% /symbolpath:"srv*C:\Programovani\Symbols*http://msdl.microsoft.com/download/symbols" 
if errorlevel 1 goto haderror





goto finished




:haderror
echo An error was encountered
pause




:finished
