// Author: Yi-Wei, Chen
// Email: yiweiche@usc.edu
// ------------------------------------
#include "ros/ros.h"
#include "ros/package.h"
#include "coordinator/coordinator_node.hpp"
#include "geometry_msgs/PoseArray.h"
#include "std_msgs/String.h"
#include "std_msgs/Int32.h"
#include "sensor_msgs/JointState.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseArray.h"
#include "std_msgs/Float32MultiArray.h"
#include <string>
#include <bits/stdc++.h> 
#include <iostream>
#include <ctime>
#include <sstream>
#include <cstdlib>

Coordinator::Coordinator(std::string use_eul_for_grasp, std::string grasp_pts_wrt_mold, std::string network_path)
{
    rviz_button_panel_sub = n_coord.subscribe("gui_coord_ctrl", 1000, &Coordinator::buttonCallback, this);
    robot_viz_pub = n_coord.advertise<std_msgs::String>("robot_viz_pub", 1000); 
    tool_viz_pub = n_coord.advertise<std_msgs::String>("tool_viz_pub", 1000);
    CADSoft_pub = n_coord.advertise<std_msgs::String>("CADSoft_pub", 1000);
    simulation_pub = n_coord.advertise<std_msgs::String>("simulation_pub", 1000);
    cal_pub = n_coord.advertise<std_msgs::String>("calculate_pub", 1000);
    robot_enable_pub = n_coord.advertise<std_msgs::String>("robot_enable_pub", 1000);
    robot_run_pub = n_coord.advertise<std_msgs::String>("robot_run_pub", 1000);

    coodinator_msgs_to_gui_pub = n_coord.advertise<std_msgs::String>("coodinator_msgs_to_gui", 1000); 
    coodinator_data_to_gui_pub = n_coord.advertise<std_msgs::String>("coodinator_data_to_gui", 1000); 

    Use_eul_for_grasp = use_eul_for_grasp;
    Grasp_pts_wrt_mold = grasp_pts_wrt_mold;
    Network_path = network_path;
};

void Coordinator::buttonCallback(const std_msgs::String& str_in)
{
    if (str_in.data.find("selected_robot_") != std::string::npos) 
    {
        robotSelectionCallback(str_in.data.substr(15));
    }

    if (str_in.data.find("selected_tool_") != std::string::npos) 
    {
        toolSelectionCallback(str_in.data.substr(14));
    }

    if (str_in.data.compare("object_import_sequence")==0) 
    {
        objectImport();
    }

    if (str_in.data.compare("calculate_sequence")==0) 
    {
        calculate_pts();
    }


    if (str_in.data.compare("Trajectory_simulation_sequence")==0) 
    {
        simulation_exe();
    }

    if (str_in.data.compare("robot_enable_sequence")==0) 
    {
        robot_enable_call();
    }


    if (str_in.data.compare("robot_running_sequence")==0) 
    {
        trajectory_run();
    }

};

void Coordinator::robotSelectionCallback(std::string part_name)
{
    system("rosnode kill robot_state_publisher");
    n_coord.deleteParam("robot_description");
    std::cout << "------------------------" << std::endl;
    std::cout << "Selected robot is : " << part_name << std::endl;
    std::cout << "------------------------" << std::endl;
    robot_name.data = part_name;
    int pos=part_name.length();
    if(part_name.find("motoman_")!= std::string::npos)
    {
        n_coord.setParam("robot_brand","motoman");
        n_coord.setParam("robot_type",part_name.substr(8,pos));
        robot_viz_pub.publish(robot_name);
    }
    else if(part_name.find("abb_")!= std::string::npos)
    {
        n_coord.setParam("robot_brand","abb");
        n_coord.setParam("robot_type",part_name.substr(4,pos));
        robot_viz_pub.publish(robot_name);
    }
    else if(part_name.find("iiwa")!= std::string::npos)
    {
        n_coord.setParam("robot_brand","iiwa");
        n_coord.setParam("robot_type",part_name.substr(0,pos));
        robot_viz_pub.publish(robot_name);
    }
    

}

void Coordinator::toolSelectionCallback(std::string tool)
{

    std::cout << "------------------------" << std::endl;
    std::cout << "Selected tool is : " << tool << std::endl;
    std::cout << "------------------------" << std::endl;
    tool_name.data = tool;
    tool_viz_pub.publish(tool_name);
 
}

void Coordinator::objectImport()
{
    std::cout << "------------------------" << std::endl;
    std::cout << "Opening feature selection window...";
    std::cout << "------------------------" << std::endl;
    CAD_trigger.data= "start";
    CADSoft_pub.publish(CAD_trigger);
}

void Coordinator::calculate_pts()
{
    std::cout << "------------------------" << std::endl;
    std::cout << "Calculating...";
    std::cout << "------------------------" << std::endl;
    cal_msgs.data="start_cal";
    cal_pub.publish(cal_msgs);
}


void Coordinator::simulation_exe()
{
    std::cout << "------------------------" << std::endl;
    std::cout << "Opening simulation window...";
    std::cout << "------------------------" << std::endl;
    simulation_msg.data="start_sim";
    simulation_pub.publish(simulation_msg);
}

void Coordinator::robot_enable_call()
{
    std::cout << "------------------------" << std::endl;
    std::cout << "/robot_enable service call...";
    std::cout << "------------------------" << std::endl;
    robot_enable_msg.data="robot_enable";
    robot_enable_pub.publish(robot_enable_msg);
}

void Coordinator::trajectory_run()
{
    std::cout << "------------------------" << std::endl;
    std::cout << "/robot_running...";
    std::cout << "------------------------" << std::endl;
    robot_run_msg.data="robot_run";
    robot_run_pub.publish(robot_run_msg);
}



   

int main(int argc, char **argv)
{
	std::cout << "\n\n\n" << std::endl;
	std::cout << "####################" << std::endl;
	std::cout << "Program Starting" << std::endl;
	std::cout << "####################" << std::endl;
	std::cout << "\n\n\n" << std::endl;

    ros::init(argc, argv, "Coordinator");
    std::string use_eul_for_grasp;
    std::string grasp_pts_wrt_mold;
    std::string network_path;


        use_eul_for_grasp = "no";    
        grasp_pts_wrt_mold = "no";  
        network_path = "no";
  
  	Coordinator CoordObj(use_eul_for_grasp, grasp_pts_wrt_mold, network_path);

  	while(ros::ok())
  	{
  		ros::Rate loop_rate(100);

  		ros::spinOnce();
  		loop_rate.sleep();

  	}

	return 0;
}