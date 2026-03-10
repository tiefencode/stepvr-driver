# stepvr openvr driver template

minimal external openvr driver for a treadmill/step-controller style device (External HID controller).

the driver exposes a single virtual controller with joystick x/y input. for v1, the live input comes from a tiny udp bridge on `127.0.0.1:54873`.

## intended data flow

HID controller standard input via BLE -> windows bridge -> udp -> openvr driver -> steamvr bindings -> game locomotion

## openvr sdk

clone the official sdk:

- repo: `https://github.com/ValveSoftware/openvr`

set an environment variable to that checkout before building:

```bat
set OPENVR_SDK_ROOT=C:\dev\openvr
```

Expected files used by CMake:

- %OPENVR_SDK_ROOT%\headers\openvr_driver.h
- %OPENVR_SDK_ROOT%\lib\win64\openvr_api.lib

## Build locally on Windows

```bat
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

Build output ends up under:

- `build\bin\win64\driver_stepvr.dll`
- `build\resources\...`
- `build\driver.vrdrivermanifest`

## Register in SteamVR

Copy or keep the whole built folder together, then run:

```bat
scripts\register_driver.bat
```

That script assumes SteamVR is installed at:

`C:\Program Files (x86)\Steam\steamapps\common\SteamVR`

If needed, edit the path inside the bat file.

## Test input without an external HID controler

Run:

```bat
python tools/test_sender.py
```

This sends a smooth forward/back test signal over UDP so you can see whether the virtual joystick value reaches SteamVR.

## Role

This template uses TrackedControllerRole_Treadmill as role hint.
That is meant for treadmill / locomotion-style devices.

If a game ignores it, a pragmatic fallback is to change the role hint to LeftHand and test again.

## Notes

- The driver deliberately does not talk to BLE directly.

- `RunFrame()` should stay lightweight.

- The UDP listener runs in a worker thread and only updates a shared state.

- Pose is fixed/minimal for now and only meant to keep the controller alive in SteamVR.