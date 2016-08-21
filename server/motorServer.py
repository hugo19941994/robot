"""Imports"""
import gc
import serial
import socket

#UDP_IP = "192.168.0.249"
UDP_IP = "192.168.43.102"
UDP_PORT = 5000


def main():
    """Sends udp messages (from server) to serial (arduino)"""

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT))
    #ser = serial.Serial('/dev/serial/by-id/usb-Arduino__www.arduino.org__Arduino_Due_Prog._Port_8543833323635131F042-if00', 19200)
    ser = serial.Serial('/dev/serial/by-id/usb-Arduino_Srl_Arduino_Mega_85439313330351F060A1-if00', 19200)
    while True:
        data, addr = sock.recvfrom(1024)
        print(data)
        ser.write(data)
main()
