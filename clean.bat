del *.ncb
del *.ilk
del *.lib
del *.exp
del *.map
del *.pdb
del log.txt
del *.bsc
del applog.txt
del *.suo /AH
del *.user
del debug\*.* /Q
del release\*.* /Q
rd release /Q
rd debug /Q

cd MCServer
del MCServer.exe
del MCServer_debug.exe
cd ..

cd VC2010
call clean.bat
cd ..