@echo off

rem Install destination folders
set OPENSOAPDRIVE=C:
set OPENSOAPBASE=%OPENSOAPDRIVE%\OpenSOAP
set OPENSOAPBIN=%OPENSOAPBASE%\bin
set OPENSOAPLIB=%OPENSOAPBASE%\lib
set OPENSOAPDLLLIB=%OPENSOAPLIB%\DLL
set OPENSOAPINC=%OPENSOAPBASE%\include
set OPENSOAPINCOPENSOAP=%OPENSOAPINC%\OpenSOAP

rem OpenSOAPClient
set OPENSOAPCLIENTDLL=Release_DLL\OpenSOAPClient.dll
set OPENSOAPCLIENTDLLLIB=Release_DLL\OpenSOAPClient.lib
set OPENSOAPCLIENTLIB=Release\OpenSOAPClient.lib
rem OpenSOAPService
set OPENSOAPSERVICEDLL=Release_DLL\OpenSOAPService.dll
set OPENSOAPSERVICEDLLLIB=Release_DLL\OpenSOAPService.lib
set OPENSOAPSERVICELIB=Release\OpenSOAPService.lib
rem OpenSOAPSecurity
set OPENSOAPSECURITYDLL=Release_DLL\OpenSOAPSecurity.dll
set OPENSOAPSECURITYDLLLIB=Release_DLL\OpenSOAPSecurity.lib
set OPENSOAPSECURITYLIB=Release\OpenSOAPSecurity.lib
rem include
set OPENSOAPINCSRC=..\..\include\OpenSOAP

echo create install folders
for %%d in (%OPENSOAPBASE% %OPENSOAPBIN% %OPENSOAPLIB% %OPENSOAPDLLLIB% %OPENSOAPINC% %OPENSOAPINCOPENSOAP%) do if not exist %%d mkdir %%d

echo install OpenSOAPClient libraries
if exist %OPENSOAPCLIENTDLL% copy %OPENSOAPCLIENTDLL% %OPENSOAPBIN% /Y
if exist %OPENSOAPCLIENTLIB% copy %OPENSOAPCLIENTLIB% %OPENSOAPLIB% /Y
if exist %OPENSOAPCLIENTDLLLIB% copy %OPENSOAPCLIENTDLLLIB% %OPENSOAPDLLLIB% /Y

echo install OpenSOAPService libraries
if exist %OPENSOAPSERVICEDLL% copy %OPENSOAPSERVICEDLL% %OPENSOAPBIN% /Y
if exist %OPENSOAPSERVICELIB% copy %OPENSOAPSERVICELIB% %OPENSOAPLIB% /Y
if exist %OPENSOAPSERVICEDLLLIB% copy %OPENSOAPSERVICEDLLLIB% %OPENSOAPDLLLIB% /Y

echo install OpenSOAPSecurity libraries
if exist %OPENSOAPSECURITYDLL% copy %OPENSOAPSECURITYDLL% %OPENSOAPBIN% /Y
if exist %OPENSOAPSECURITYLIB% copy %OPENSOAPSECURITYLIB% %OPENSOAPLIB% /Y
if exist %OPENSOAPSECURITYDLLLIB% copy %OPENSOAPSECURITYDLLLIB% %OPENSOAPDLLLIB% /Y

echo install OpenSOAP Header files
copy %OPENSOAPINCSRC%\*.h %OPENSOAPINCOPENSOAP% /Y




