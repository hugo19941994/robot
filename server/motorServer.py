"""Imports"""
import gc
import serial
import socket

UDP_IP = "127.0.0.1"
UDP_PORT = 5000


def main():
    """Sends udp messages (from client) to serial (arduino)"""

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT))
    ser = serial.Serial('/dev/ttyUSB0', 19200)
    while True:
        data, addr = sock.recvfrom(1024)
        ser.write(data)
main()
