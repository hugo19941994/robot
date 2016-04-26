import serial
import time

def main():
    ser = serial.Serial('/dev/ttyUSB0', 9600)
    clear_buffer = time.time() + 60
    buffer = ''
    while True:
        if time.time() >= clear_buffer:
            print(len(buffer))
            buffer = ''
            clear_buffer = time.time() + 60
        buffer += str(ser.read().decode("utf-8"))
        print(buffer)

main()
