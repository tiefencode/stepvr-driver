#include "step_provider.h"

StepProvider::StepProvider() = default;
StepProvider::~StepProvider() = default;

vr::EVRInitError StepProvider::Init(vr::IVRDriverContext* pDriverContext) {
    VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);

    m_controller = std::make_unique<StepController>(m_bridgeState);
    m_bridge = std::make_unique<UdpBridge>(m_bridgeState, "127.0.0.1", 54873);
    m_bridge->start();

    vr::VRServerDriverHost()->TrackedDeviceAdded(
        "stepvr_001",
        vr::TrackedDeviceClass_Controller,
        m_controller.get());

    return vr::VRInitError_None;
}

void StepProvider::Cleanup() {
    if (m_bridge) {
        m_bridge->stop();
        m_bridge.reset();
    }
    m_controller.reset();
    VR_CLEANUP_SERVER_DRIVER_CONTEXT();
}

const char* const* StepProvider::GetInterfaceVersions() {
    return vr::k_InterfaceVersions;
}

void StepProvider::RunFrame() {
    vr::VREvent_t event;
    while (vr::VRServerDriverHost()->PollNextEvent(&event, sizeof(event))) {
    }

    if (m_controller) {
        m_controller->RunFrame();
    }
}

bool StepProvider::ShouldBlockStandbyMode() {
    return false;
}

void StepProvider::EnterStandby() {}
void StepProvider::LeaveStandby() {}