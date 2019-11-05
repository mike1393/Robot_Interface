// Author: Yi-Wei, Chen
// Email: yiweiche@usc.edu
// ------------------------------------
#ifndef COORDINATOR_NODE
#define COORDINATOR_NODE

#include "ros/ros.h"
#include "ros/package.h"
#include "geometry_msgs/PoseArray.h"
#include "std_msgs/String.h"
#include "std_msgs/Int32.h"
#include "sensor_msgs/JointState.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseArray.h"
#include <string>
#include <bits/stdc++.h> 
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "std_msgs/Float32MultiArray.h"

class Coordinator
{
public:

	ros::NodeHandle n_coord;
    ros::Subscriber rviz_button_panel_sub;
    ros::Publisher robot_viz_pub;    
    ros::Publisher tool_viz_pub;
    ros::Publisher CADSoft_pub;
    ros::Publisher cal_pub;
    ros::Publisher simulation_pub;
    ros::Publisher robot_enable_pub;
    ros::Publisher robot_run_pub;
    ros::Publisher coodinator_msgs_to_gui_pub;
    ros::Publisher coodinator_data_to_gui_pub;
 
    std_msgs::String robot_name;
    std_msgs::String tool_name;
    std_msgs::String CAD_trigger;
    std_msgs::String simulation_msg;
    std_msgs::String cal_msgs;
    std_msgs::String robot_enable_msg;
    std_msgs::String robot_run_msg;
    std::string Use_eul_for_grasp;
    std::string Grasp_pts_wrt_mold;
    std::string Network_path;
        
    Coordinator(std::string, std::string, std::string);

    void buttonCallback(const std_msgs::String&);
    
    void robotSelectionCallback(std::string);

    void toolSelectionCallback(std::string);

    void objectImport();

    void calculate_pts();

    void simulation_exe();

    void robot_enable_call();

    void trajectory_run();

    
    void publishToIntervController();


   
};

#endif