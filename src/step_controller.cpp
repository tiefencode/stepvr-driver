#include "step_controller.h"
#include <cstring>

StepController::StepController(BridgeState& state)
    : m_state(state) {}

vr::EVRInitError StepController::Activate(uint32_t unObjectId) {
    m_objectId = unObjectId;
    m_props = vr::VRProperties()->TrackedDeviceToPropertyContainer(m_objectId);

    vr::VRProperties()->SetStringProperty(m_props, vr::Prop_ModelNumber_String, "stepvr controller");
    vr::VRProperties()->SetStringProperty(m_props, vr::Prop_RenderModelName_String, "vr_controller_vive_1_5");
    vr::VRProperties()->SetStringProperty(m_props, vr::Prop_SerialNumber_String, m_serial.c_str());
    vr::VRProperties()->SetStringProperty(m_props, vr::Prop_ManufacturerName_String, "stepvr");
    vr::VRProperties()->SetStringProperty(m_props, vr::Prop_ControllerType_String, "stepvr");
    vr::VRProperties()->SetStringProperty(m_props, vr::Prop_InputProfilePath_String, "{stepvr}/input/stepvr_profile.json");
    vr::VRProperties()->SetInt32Property(m_props, vr::Prop_ControllerRoleHint_Int32, vr::TrackedControllerRole_Treadmill);

    vr::VRDriverInput()->CreateScalarComponent(
        m_props, "/input/joystick/x", &m_joyX,
        vr::VRScalarType_Absolute, vr::VRScalarUnits_NormalizedTwoSided);

    vr::VRDriverInput()->CreateScalarComponent(
        m_props, "/input/joystick/y", &m_joyY,
        vr::VRScalarType_Absolute, vr::VRScalarUnits_NormalizedTwoSided);

    vr::VRDriverInput()->CreateBooleanComponent(
        m_props, "/input/joystick/click", &m_joyClick);

    return vr::VRInitError_None;
}

void StepController::Deactivate() {
    m_objectId = vr::k_unTrackedDeviceIndexInvalid;
    m_props = vr::k_ulInvalidPropertyContainer;
}

void StepController::EnterStandby() {}

void* StepController::GetComponent(const char* pchComponentNameAndVersion) {
    return nullptr;
}

void StepController::DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) {
    if (unResponseBufferSize >= 1) {
        pchResponseBuffer[0] = '\0';
    }
}

vr::DriverPose_t StepController::GetPose() {
    return MakePose();
}

vr::DriverPose_t StepController::MakePose() const {
    vr::DriverPose_t pose{};
    pose.deviceIsConnected = true;
    pose.poseIsValid = true;
    pose.result = vr::TrackingResult_Running_OK;

    pose.qWorldFromDriverRotation.w = 1.0;
    pose.qDriverFromHeadRotation.w = 1.0;
    pose.qRotation.w = 1.0;

    pose.vecPosition[0] = 0.0;
    pose.vecPosition[1] = 1.0;
    pose.vecPosition[2] = 0.5;
    return pose;
}

void StepController::UpdateInputs() {
    if (m_objectId == vr::k_unTrackedDeviceIndexInvalid) return;

    const float x = m_state.joyX.load();
    const float y = m_state.joyY.load();
    const bool click = m_state.joyClick.load();

    vr::VRDriverInput()->UpdateScalarComponent(m_joyX, x, 0.0);
    vr::VRDriverInput()->UpdateScalarComponent(m_joyY, y, 0.0);
    vr::VRDriverInput()->UpdateBooleanComponent(m_joyClick, click, 0.0);

    auto pose = MakePose();
    vr::VRServerDriverHost()->TrackedDevicePoseUpdated(m_objectId, pose, sizeof(vr::DriverPose_t));
}

void StepController::RunFrame() {
    UpdateInputs();
}