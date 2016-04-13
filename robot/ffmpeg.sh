    timeout 2s $(ffmpeg -re -loop 1 -i /home/hfs/img.jpg -f mpegts udp://127.0.0.1:1234)
