import numpy as np
import os
import io
from PIL import Image
from subprocess import Popen, PIPE
from time import sleep
import matplotlib.pyplot as plt
import serial
from scipy.interpolate import interp1d
from scipy.misc import toimage


if __name__ == '__main__':

    ser = serial.Serial(
        '/dev/serial/by-id/'
        'usb-Arduino__www.arduino.org__Arduino_Due_Prog._Port_8543833323635131F042-if00',
        115200)
    i = 0

    while(True):
        ser.write('A'.encode())
        print("waiting")
        sleep(0.10)
        if (ser.in_waiting > 100):
            camera_image = ser.read(24000)
            camera_image = str(camera_image)
            camera_image = camera_image[2:-2]
            print(camera_image)

            camera_image_array = np.array(camera_image.split(" "))
            print(camera_image_array)
            print(camera_image_array.size)
            for idx, data in enumerate(camera_image_array):
                if not data.isdigit():
                    camera_image_array[idx] = "8000"
            cam3 = camera_image_array.astype(np.int)
            for idx, data in enumerate(cam3):
                if data > 8500 or data < 7800:
                    cam3[idx] = 8000
            m = interp1d([7800, 8500], [0, 1])
            if cam3.size > 4800:
                cam3 = cam3[:-(4800-cam3.size)]
            cam4 = np.reshape(cam3, (60, 80))
            cam5 = m(cam4)
            cam5 = cam5*255

            img = toimage(cam5)
            img = img.resize((800, 600))
            plt.figure(frameon=False)
            plt.imshow(img)
            plt.savefig('/home/hfs/img2.jpg')
            #img.save('/home/hfs/img.jpg', 'JPEG')
            i = i + 1
