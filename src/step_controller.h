#pragma once

#include "bridge_udp.h"
#include <openvr_driver.h>
#include <string>

class StepController final : public vr::ITrackedDeviceServerDriver {
public:
    StepController(BridgeState& state);
    ~StepController() = default;

    vr::EVRInitError Activate(uint32_t unObjectId) override;
    void Deactivate() override;
    void EnterStandby() override;
    void* GetComponent(const char* pchComponentNameAndVersion) override;
    void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) override;
    vr::DriverPose_t GetPose() override;

    void RunFrame();

private:
    void UpdateInputs();
    vr::DriverPose_t MakePose() const;

    BridgeState& m_state;
    uint32_t m_objectId = vr::k_unTrackedDeviceIndexInvalid;
    vr::PropertyContainerHandle_t m_props = vr::k_ulInvalidPropertyContainer;
    vr::VRInputComponentHandle_t m_joyX = vr::k_ulInvalidInputComponentHandle;
    vr::VRInputComponentHandle_t m_joyY = vr::k_ulInvalidInputComponentHandle;
    vr::VRInputComponentHandle_t m_joyClick = vr::k_ulInvalidInputComponentHandle;
    std::string m_serial = "stepvr_001";
};