@
@ Profiling using a MSVC standalone profiler
@
@ See http://www.codeproject.com/Articles/144643/Profiling-of-C-Applications-in-Visual-Studio-for-F for details
@




set pt="C:\Program Files\Microsoft Visual Studio 9.0\Team Tools\Performance Tools"
set app="vc2008\Release profiled\mcserver.exe"
set output=MCServer_profiledata.vsp





cd ..

:: Start the profiler
%pt%\vsperfcmd /start:sample /output:%output%

:: Launch the application via the profiler
%pt%\vsperfcmd /launch:%app%

:: Shut down the profiler (this command waits, until the application is terminated)
%pt%\vsperfcmd /shutdown





:: generate the report files (.csv)
%pt%\vsperfreport /summary:all %output% /symbolpath:"srv*C:\Programovani\Symbols*http://msdl.microsoft.com/download/symbols" 





