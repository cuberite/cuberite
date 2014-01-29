@echo off
:: MakeLuaAPI.cmd
:: This script is run after the nightbuild to produce the Lua API documentation and upload it to a website.
:: It expects at least three environment variables set: ftpsite, ftpuser and ftppass, specifying the FTP site and login to use for the upload





:: Check that we got all the environment vars needed for the upload:

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





:: Create the API documentation by running the server and stopping it right after it starts:

cd MCServer
copy /Y settings_apidump.ini settings.ini
echo stop | MCServer
cd ..





:: Upload the API to the web:

ncftpput -p %ftppass% -u %ftpuser% -T temp_ -R %ftpsite% /LuaAPI MCServer/API/*.*
if errorlevel 1 goto haderror
echo Upload finished.

goto end





:haderror
echo an error was encountered, check command output above
pause
goto finished





:end
if "a%1" == "a" pause



:finished