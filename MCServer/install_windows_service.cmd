rem Alter this if you need to install multiple instances.
@echo off
set SERVICENAME="MCServer"

set CURRENTDIR=%CD%
sc create %SERVICENAME% binPath= "%CURRENTDIR%\MCServer.exe -d" start= auto DisplayName= %SERVICENAME%
sc description %SERVICENAME% "Minecraft server instance"