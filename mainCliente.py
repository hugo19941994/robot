"""Imports"""
import gc
import serial
import socket

UDP_IP = "127.0.0.1"
UDP_PORT = 5000


def main():
    """Sends udp messages (from server) to serial (arduino)"""

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT))
    ser = serialSerial('/dev/ttyUSB0', 115200)
    while True:
        data, addr = sock.recvfrom(1024)
        ser.write(data)
main()
