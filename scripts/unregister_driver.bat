@echo off
setlocal

set "STEAMVR_ROOT=C:\Program Files (x86)\Steam\steamapps\common\SteamVR"
set "VRPATHREG=%STEAMVR_ROOT%\bin\win64\vrpathreg.exe"
set "DRIVER_ROOT=%~dp0"
if "%DRIVER_ROOT:~-1%"=="\" set "DRIVER_ROOT=%DRIVER_ROOT:~0,-1%"

if not exist "%VRPATHREG%" goto :missing_vrpathreg

call "%VRPATHREG%" removedriver "%DRIVER_ROOT%"
call "%VRPATHREG%" finddriver stepvr
goto :done

:missing_vrpathreg
echo vrpathreg.exe not found at %VRPATHREG%
exit /b 1

:done
endlocal