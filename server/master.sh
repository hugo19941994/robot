$(python3 motorServer.py &)
$(sh ./kinect/send.sh &)
$(sh ./kinect/odom.sh &)
#$(sh ./video/ir/irCamera.sh &)
$(sh ./video/ir/irStream.sh &)
$(sh ./video/web/webcam.sh &)
python3 ./sensors/cpm.py
