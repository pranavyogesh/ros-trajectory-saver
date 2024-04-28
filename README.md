ROS package to visualize and save robot trajectory. Displays Marker objects on Rviz to show points. Creates onemarker for coordinates after every 0.1m displacement, and updates it to a .csv file.

Setup:

Clone the repo

`catkin_make`

`source/devel/setup.bash`

Terminal 1:

`export TURTLEBOT3_MODEL=burger`

`roslaunch turtlebot3_gazebo turtlebot3_empty_world.launch`

Terminal 2:
`roslaunch trajectory_tracker tf.launch`

Terminal 3:
`rosrun trajectory_tracker marker_publisher`

Coordinates are saved in the marker_array.csv file in catkin_ws/

Other commands

`rosrun rviz rviz`

`rosrun teleop_twist_keyboard teleop_twist_keyboard.py`
