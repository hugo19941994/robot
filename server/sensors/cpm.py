import serial
import time

def main():
    ser = serial.Serial('/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A104OWAZ-if00-port0', 9600)
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

