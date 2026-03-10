@echo off
setlocal
set STEAMVR_ROOT=C:\Program Files (x86)\Steam\steamapps\common\SteamVR
set VRPATHREG=%STEAMVR_ROOT%\bin\win64\vrpathreg.exe
set DRIVER_ROOT=%~dp0

if not exist "%VRPATHREG%" (
  echo vrpathreg.exe not found at %VRPATHREG%
  exit /b 1
)

"%VRPATHREG%" adddriver "%DRIVER_ROOT%"
"%VRPATHREG%" finddriver stepvr
endlocal