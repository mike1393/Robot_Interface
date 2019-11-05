// Author: Yi-Wei, Chen
// Email: yiweiche@usc.edu
// ------------------------------------
#include <ros/ros.h>
#include <rviz_visual_tools/rviz_visual_tools.h>
#include "std_msgs/String.h"
#include <geometry_msgs/Pose.h>
#include <string>
#include <vector>
#include <iostream>
#include <urdf/model.h>
#include <fstream>
#include <ros/package.h>

class Calculator
{
public:

	ros::NodeHandle n_cal_viz;
	ros::Subscriber cal_name_sub;
	ros::Publisher cal_pub;
	rviz_visual_tools::RvizVisualToolsPtr visual_tools_;
	std::string robot_info;

	std::string urdf_file;

	Calculator()
	{
		cal_name_sub 
        = n_cal_viz.subscribe("calculate_pub", 1000, &Calculator::getCALwindow, this);



        cal_pub=n_cal_viz.advertise<std_msgs::String>("coodinator_msgs_to_gui",1000);
	}
	~Calculator()
	{
		cal_name_sub.shutdown();

	}
	void getCALwindow(const std_msgs::String& sim_name)
	{

		if (sim_name.data.compare("start_cal")==0)
		{
			std_msgs::String msg;
            msg.data = "Calcualtion start...\n";
            cal_pub.publish(msg);
			ROS_INFO_ONCE("Calcualtion start...");
			n_cal_viz.getParam("robot_type",robot_info);
			std::string package_name = "motoman_app";
			std::string str= "rosrun object_viz tool_shift && rosrun motoman_app ik_rotation_input.py && rosrun motoman_app ik_link_trans.py"; 
			const char* command=str.c_str(); 
			system(command);
			ROS_INFO_ONCE("Calculation end...");
			sleep(1);
		}

    }


};

int main(int argc, char** argv)
{


  	ros::init(argc, argv, "cal_Viz");
	ROS_INFO_STREAM("Opening seperate window...");
    
		Calculator simObj;    

    while(ros::ok())
    {
    	ros::Rate loop_rate(1000);

  		ros::spinOnce();
  		loop_rate.sleep();
    }  
return 0;
}