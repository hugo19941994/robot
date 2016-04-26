"""Imports"""
import gc
import socket
from evdev import InputDevice


def main():
    """Checks PS3 inputs and sends them via UDP"""
    dev = InputDevice('/dev/input/by-id/usb-Sony_PLAYSTATION_R_3_Controller-event-joystick')
    speed1 = 0
    speed2 = 0
    speed3 = 0
    dir1 = 'F'
    dir2 = 'F'
    dir3 = 'F'

    print(dev)
    print(dev.capabilities(verbose=True, absinfo=False))

    UDP_IP = "192.168.0.249"
    UDP_PORT = 5000

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    while True:
        event = dev.read()
        try:
            for evn in event:
                if evn.code == 1:  # right x
                    # print(ev)
                    speed1 = evn.value - 128
                    if speed1 < 0:
                        dir1 = 'F'
                        if speed1 == -128:
                            speed1 = -127
                        # ser.write('AF'.encode() + str(abs(dir * 2)).encode())
                        print("Motor A - Forward - " + str(abs(speed1 * 2)))
                    else:
                        dir1 = 'B'
                        # ser.write('AB'.encode() + str(dir).encode())
                        print("Motor A - Backward - " + str(abs(speed1 * 2)))
                    # ser.write('A'.encode() + str(evn.value - 128).encode())
                elif evn.code == 5:  # left x
                    speed2 = evn.value - 128
                    # print(ev)
                    if speed2 < 0:
                        dir2 = 'F'
                        if speed2 == -128:
                            speed2 = -127
                        # ser.write('BF'.encode() + str(abs(dir * 2)).encode())
                        print("Motor B - Forward - " + str(abs(speed2 * 2)))
                    else:
                        dir2 = 'B'
                        # ser.write('BB'.encode() + str(dir * 2).encode())
                        print("Motor B - Backward - " + str(abs(speed2 * 2)))
                elif evn.code == 300:  # left x
                    print("motor 3 F")
                    speed3 = 127
                    dir3 = 'F'
                elif evn.code == 302:
                    print("motor 3 B")
                    speed3 = 127
                    dir3 = 'B'
                elif evn.code == 301:
                    speed3 = 0
            sock.sendto(dir1.encode() +
                        str(abs(speed1 * 2)).zfill(3).encode() +
                        dir2.encode() +
                        str(abs(speed2 * 2)).zfill(3).encode() +
                        dir3.encode() +
                        str(abs(speed3 * 2)).zfill(3).encode() +
                        str(0).encode() +
                        "\n".encode(), (UDP_IP, UDP_PORT))
            gc.collect()
        except IOError:
            pass
main()
