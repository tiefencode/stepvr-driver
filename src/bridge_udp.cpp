#include "bridge_udp.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdint>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

namespace {
#pragma pack(push, 1)
struct Packet {
    float x;
    float y;
    uint8_t click;
};
#pragma pack(pop)
}

UdpBridge::UdpBridge(BridgeState& state, const char* ip, int port)
    : m_state(state), m_ip(ip), m_port(port) {}

UdpBridge::~UdpBridge() {
    stop();
}

bool UdpBridge::start() {
    if (m_running.load()) return true;

    WSADATA wsa{};
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        return false;
    }

    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == INVALID_SOCKET) {
        WSACleanup();
        return false;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<u_short>(m_port));
    inet_pton(AF_INET, m_ip, &addr.sin_addr);

    if (bind(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
        closesocket(s);
        WSACleanup();
        return false;
    }

    m_socket = reinterpret_cast<void*>(s);
    m_running.store(true);
    m_thread = std::thread(&UdpBridge::run, this);
    return true;
}

void UdpBridge::stop() {
    if (!m_running.exchange(false)) return;

    SOCKET s = reinterpret_cast<SOCKET>(m_socket);
    if (s) {
        closesocket(s);
        m_socket = nullptr;
    }

    if (m_thread.joinable()) {
        m_thread.join();
    }

    WSACleanup();
}

void UdpBridge::run() {
    SOCKET s = reinterpret_cast<SOCKET>(m_socket);
    while (m_running.load()) {
        Packet p{};
        sockaddr_in from{};
        int fromlen = sizeof(from);
        int received = recvfrom(
            s,
            reinterpret_cast<char*>(&p),
            sizeof(p),
            0,
            reinterpret_cast<sockaddr*>(&from),
            &fromlen
        );

        if (received == sizeof(Packet)) {
            m_state.joyX.store(p.x);
            m_state.joyY.store(p.y);
            m_state.joyClick.store(p.click != 0);
        }
    }
}