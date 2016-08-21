export ROS_MASTER_URI="http://192.168.43.102:11311"
export ROS_IP=192.168.43.102
roslaunch rtabmap_ros rgbd_mapping.launch rtabmap_args:="--delete_db_on_start" rviz:=false rtabmapviz:=false

