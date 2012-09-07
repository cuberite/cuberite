@echo off
::
:: Profiling using a MSVC standalone profiler
::
:: See http://www.codeproject.com/Articles/144643/Profiling-of-C-Applications-in-Visual-Studio-for-F for details
::




set pt="C:\Program Files\Microsoft Visual Studio 9.0\Team Tools\Performance Tools"
set appdir=..\MCServer
set app=MCServer.exe

:: outputdir is relative to appdir!
set outputdir=..\Profiling
set output=%outputdir%\profile.vsp





:: Must cd to MCServer's directory so that it can find settings.ini etc.
cd %appdir%

::Create the output directory, if it didn't exist
mkdir %outputdir%





:: Start the profiler
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
%pt%\vsperfreport /summary:all %output% /symbolpath:"srv*C:\Programovani\Symbols*http://msdl.microsoft.com/download/symbols" 
if errorlevel 1 goto haderror





goto finished




:haderror
echo An error was encountered
pause




:finished
