#pragma once

#include <atomic>
#include <thread>

struct BridgeState {
    std::atomic<float> joyX{0.0f};
    std::atomic<float> joyY{0.0f};
    std::atomic<bool> joyClick{false};
};

class UdpBridge {
public:
    UdpBridge(BridgeState& state, const char* ip, int port);
    ~UdpBridge();

    bool start();
    void stop();

private:
    void run();

    BridgeState& m_state;
    const char* m_ip;
    int m_port;
    std::atomic<bool> m_running{false};
    std::thread m_thread;
    void* m_socket = nullptr;
};