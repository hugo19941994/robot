"""Imports"""
import gc
import socket
import serial
from evdev import InputDevice


def main():
    """Checks PS3 inputs and sends them via UDP"""
    dev = InputDevice('/dev/input/by-id/usb-Sony_PLAYSTATION_R_3_Controller-event-joystick')
    ser = serial.Serial('/dev/ttyACM3', 19200)

    servo1 = 90
    servo2 = 90
    servo3 = 90
    servo4 = 90
    servo5 = 90

    send = True

    print(dev)
    print(dev.capabilities(verbose=True, absinfo=False))

    UDP_IP = "192.168.0.249"
    UDP_PORT = 5000

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    while True:
        event = dev.active_keys()
        print(dev.active_keys())
        print(servo1)
        print(servo2)
        print(servo3)
        print(servo4)
        print(servo5)
        try:
            for evn in event:
                # 299 & 298
                if evn == 299:
                    send = True
                    if servo1 >= 180:
                        servo1 = 180
                        servo2 = 0
                    else:
                        servo1 += 0.5
                        servo2 -= 0.5
                elif evn == 298:
                    send = True
                    if servo1 <= 0:
                        servo1 = 0
                        servo2 = 180
                    else:
                        servo1 -= 0.5
                        servo2 += 0.5

                # 296 & 297
                if evn == 296:
                    send = True
                    if servo3 >= 180:
                        servo3 = 180
                    else:
                        servo3 += 0.5
                elif evn== 297:
                    send = True
                    if servo3 <= 0:
                        servo3 = 0
                    else:
                        servo3 -= 0.5

                # 295 & 293 - dcha izq
                if evn== 295: # dcha
                    send = True
                    if servo4 >= 180:
                        servo4 = 180
                    else:
                        servo4 += 0.5
                elif evn== 293: # izq
                    send = True
                    if servo4 <= 0:
                        servo4 = 0
                    else:
                        servo4 -= 0.5

                # 292 & 294 - arriba abajo
                if evn== 292: # arriba
                    send = True
                    if servo5 >= 180:
                        servo5 = 180
                    else:
                        servo5 += 0.5
                elif evn== 294: # abajo
                    send = True
                    if servo5 <= 0:
                        servo5 = 0
                    else:
                        servo5 -= 0.5

            if(send):
                ser.write(str(int(servo1)).zfill(3).encode() +
                        str(int(servo2)).zfill(3).encode() +
                        str(int(servo3)).zfill(3).encode() +
                        str(int(servo4)).zfill(3).encode() +
                        str(int(servo5)).zfill(3).encode() +
                        "\n".encode())
                send= False
            gc.collect()
        except IOError:
            pass
main()
