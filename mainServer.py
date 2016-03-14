"""Imports"""
import gc
import socket
from evdev import InputDevice


def main():
    """Checks PS3 inputs and sends them via UDP"""
    dev = InputDevice('/dev/input/by-id/usb-Sony_PLAYSTATION_R_3_Controller-event-joystick')
    speed1 = 0
    speed2 = 0
    dir1 = 'F'
    dir2 = 'F'
    servo1 = 0
    servo2 = 0
    s1bpress = False
    s1bpress2 = False
    s2bpress = False
    s2bpress2 = False

    print(dev)
    print(dev.capabilities(verbose=True, absinfo=False))

    UDP_IP = "127.0.0.1"
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
                elif evn.code == 299:
                    print("gatillo")
                    if servo1 + 0.5 > 180:
                        servo1 = 180
                    if s1bpress:
                        s1bpress = False
                    else:
                        servo1 += 0.5
                        s1bpress = True
                elif s1bpress:
                    servo1 += 0.5
                elif evn.code == 298:
                    if servo1 - 0.5 < 0:
                        servo1 = 0
                    if s1bpress2:
                        s1bpress2 = False
                    else:
                        servo1 -= 0.5
                        s1bpress2 = True
                elif s1bpress2:
                    servo1 -= 0.5
                if evn.code == 296:
                    if servo2 + 0.5 > 180:
                        servo2 = 180
                    if s2bpress:
                        s2bpress = False
                    else:
                        servo2 += 0.5
                        s2bpress = True
                elif s2bpress:
                    servo2 += 0.5
                elif evn.code == 297:
                    if servo2 - 0.5 < 0:
                        servo2 = 0
                    if s2bpress2:
                        s2bpress2 = False
                    else:
                        servo2 -= 0.5
                        s2bpress2 = True
                elif s2bpress2:
                    servo2 -= 0.5
            sock.sendTo(dir1.encode() +
                        str(abs(speed1 * 2)).zfill(3).encode() +
                        dir2.encode() +
                        str(abs(speed2 * 2)).zfill(3).encode() +
                        str(int(servo1)).zfill(3).encode() +
                        str(int(servo2)).zfill(3).encode() + "\n".encode(), (UDP_IP, UDP_PORT))
            gc.collect()
        except IOError:
            pass
main()
