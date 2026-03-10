#include "step_provider.h"
#include <openvr_driver.h>
#include <cstring>

static StepProvider g_provider;

#if defined(_WIN32)
#define HMD_DLL_EXPORT extern "C" __declspec(dllexport)
#else
#define HMD_DLL_EXPORT extern "C"
#endif

HMD_DLL_EXPORT void* HmdDriverFactory(const char* pInterfaceName, int* pReturnCode) {
    if (std::strcmp(vr::IServerTrackedDeviceProvider_Version, pInterfaceName) == 0) {
        return &g_provider;
    }

    if (pReturnCode) {
        *pReturnCode = vr::VRInitError_Init_InterfaceNotFound;
    }
    return nullptr;
}