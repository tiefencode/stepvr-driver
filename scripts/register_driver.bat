@echo off
setlocal

set "STEAMVR_ROOT=C:\Program Files (x86)\Steam\steamapps\common\SteamVR"
set "VRPATHREG=%STEAMVR_ROOT%\bin\win64\vrpathreg.exe"
set "SCRIPT_DIR=%~dp0"
set "DRIVER_ROOT=%SCRIPT_DIR%"

if exist "%DRIVER_ROOT%driver.vrdrivermanifest" goto driver_root_ok

set "DRIVER_ROOT=%SCRIPT_DIR%..\build"
if exist "%DRIVER_ROOT%\driver.vrdrivermanifest" goto driver_root_ok

echo could not locate driver root
echo checked:
echo   %SCRIPT_DIR%
echo   %SCRIPT_DIR%..\build
exit /b 1

:driver_root_ok
if "%DRIVER_ROOT:~-1%"=="\" set "DRIVER_ROOT=%DRIVER_ROOT:~0,-1%"

if not exist "%VRPATHREG%" goto :missing_vrpathreg

call "%VRPATHREG%" adddriver "%DRIVER_ROOT%"
call "%VRPATHREG%" finddriver stepvr
goto :done

:missing_vrpathreg
echo vrpathreg.exe not found at %VRPATHREG%
exit /b 1

:done
endlocal