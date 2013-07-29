@echo off
:: UpdateVersions.cmd
:: This script processes all *.template files into their non-templated variants using subwcrev, substituting WC-related keywords in the process
:: subwcrev is expected to be in path; you can pass the correct path for your system as env var "subwcrev"


:: Subwcrev (from TortoiseSVN, for querying revision number; by default in PATH):
if %subwcrev%a == a set subwcrev=subwcrev




:: Copy all *.template files into their non-template versions, substituting SVN keywords:
for /r %%X in (*.template) do (
	%subwcrev% . "%%X" "%%~dpX%%~nX"
	if errorlevel 1 goto haderror
)


goto end




:haderror
echo an error was encountered, check command output above
pause
goto finished





:end
if "a%1" == "a" pause



:finished