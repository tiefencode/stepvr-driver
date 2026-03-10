stepvr openvr driver template

Minimal external OpenVR driver for a treadmill / step-controller style device.

The driver exposes a single virtual controller with joystick X/Y input. For v1, the live input comes from a tiny UDP bridge on 127.0.0.1:54873.

Intended data flow

ESP32 / existing BLE gamepad logic → Windows bridge → UDP → OpenVR driver → SteamVR bindings → game locomotion

OpenVR SDK

Clone the official SDK:

https://github.com/ValveSoftware/openvr

Set an environment variable to that checkout before building:

set OPENVR_SDK_ROOT=C:\dev\openvr

Expected files used by CMake:

%OPENVR_SDK_ROOT%\headers\openvr_driver.h

%OPENVR_SDK_ROOT%\lib\win64\openvr_api.lib

Build locally on Windows
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release

Build output ends up under:

build\bin\win64\driver_stepvr.dll

build\resources\...

build\driver.vrdrivermanifest

Register in SteamVR

Copy or keep the whole built folder together, then run:

scripts\register_driver.bat

That script assumes SteamVR is installed at:

C:\Program Files (x86)\Steam\steamapps\common\SteamVR

If needed, edit the path inside the bat file.

Test input without ESP32

Run:

python tools/test_sender.py

This sends a smooth forward/back test signal over UDP so you can see whether the virtual joystick value reaches SteamVR.

Role

This template uses TrackedControllerRole_Treadmill as role hint.
That is meant for treadmill / locomotion-style devices.

If a game ignores it, a pragmatic fallback is to change the role hint to LeftHand and test again.

Notes

The driver deliberately does not talk to BLE directly.

RunFrame() should stay lightweight.

The UDP listener runs in a worker thread and only updates a shared state.

Pose is fixed/minimal for now and only meant to keep the controller alive in SteamVR.