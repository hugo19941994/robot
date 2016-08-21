ffmpeg -f alsa -ac 1 -i hw:1,0 -i /dev/video0 -vf "transpose=1" -vcodec libx264 -r 10 -s 320x240 -tune zerolatency -f mpegts udp://192.168.43.161:1234
