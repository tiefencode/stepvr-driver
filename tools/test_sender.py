import math
import socket
import struct
import time

HOST = "127.0.0.1"
PORT = 54873
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

t = 0.0
print(f"sending test joystick values to {HOST}:{PORT}")
while True:
    y = math.sin(t) * 0.75
    x = 0.0
    click = 1 if abs(y) > 0.6 else 0
    packet = struct.pack("<ffB", x, y, click)
    sock.sendto(packet, (HOST, PORT))
    print(f"x={x:.2f} y={y:.2f} click={click}")
    time.sleep(0.05)
    t += 0.08