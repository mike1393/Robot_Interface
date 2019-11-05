# Robot_Interface
ROS package for robot trajectroy simulation<br>
### Package includes 
+ Robot Selection (Support ABB, Motoman)
+ Tool Selection
+ Workpiece Selection
+ Joint Trajectory File Input
+ Tool Path File Input
### Setup 
ROS Kinetic, Ubuntu 16.04 LTS
<br>
### Maintainer 
Yi-Wei (Mike), Chen
## Installation 
### Requirements 
##### ROS packages 
+ [ROS](http://wiki.ros.org/ROS/Tutorials/InstallingandConfiguringROSEnvironment)<br>
+ [ROS-Industrial](http://wiki.ros.org/Industrial/Install)<br>
+ [RViz](http://wiki.ros.org/rviz/UserGuide)<br>
##### Robot packages
Go to your catkin workspace:<br>
<code>cd ~/catkin_ws/src</code><br>
Clone these repositories:<br>
+ **ABB**: <br>
<code>git clone https://github.com/ros-industrial/abb.git </code><br>
+ **Motoman**: <br>
<code>git clone https://github.com/ros-industrial/motoman.git </code><br>
##### Kinematic packages
Clone these repositories to your src directory:<br>
**hrl-kdl**<br>
<code>git clone https://github.com/gt-ros-pkg/hrl-kdl.git </code><br>
### Package Installation 
Go to your catkin workspace:<br>
<code>cd ~/catkin_ws/src</code><br>
Clone this repository:<br>
<code>git clone https://github.com/mike1393/Robot_Interface.git</code><br>
Now compile your workspace:<br>
<code>
cd ~/catkin_ws 
</code><br>
<code>
catkin_make
</code><br>
