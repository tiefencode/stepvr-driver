#pragma once

#include "bridge_udp.h"
#include "step_controller.h"
#include <memory>
#include <openvr_driver.h>

class StepProvider final : public vr::IServerTrackedDeviceProvider {
public:
    StepProvider();
    ~StepProvider() override;

    vr::EVRInitError Init(vr::IVRDriverContext* pDriverContext) override;
    void Cleanup() override;
    const char* const* GetInterfaceVersions() override;
    void RunFrame() override;
    bool ShouldBlockStandbyMode() override;
    void EnterStandby() override;
    void LeaveStandby() override;

private:
    BridgeState m_bridgeState;
    std::unique_ptr<UdpBridge> m_bridge;
    std::unique_ptr<StepController> m_controller;
};