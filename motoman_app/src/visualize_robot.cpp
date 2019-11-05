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

class RobotVisualizer
{
public:

	ros::NodeHandle n_robot_viz;
	ros::Subscriber robot_name_sub;
	ros::Publisher robot_pub;
	rviz_visual_tools::RvizVisualToolsPtr visual_tools_;
	std::string robot_info;
	std::string robot_brand;
	std::string urdf_file;

	RobotVisualizer()
	{
        robot_name_sub = n_robot_viz.subscribe("robot_viz_pub", 1000, &RobotVisualizer::getRobotInfo, this);
	};
	~RobotVisualizer()
	{
		n_robot_viz.deleteParam("robot_description");
		system("rosnode kill robot_state_publisher");
	}
	void getRobotInfo(const std_msgs::String& robot_name)
	{
		ROS_INFO_STREAM(robot_name.data);

		if (robot_name.data.size()!=0)
		{
			// n_robot_viz.getParam("robot_brand",robot_brand);
			// if(robot_brand=="motoman")
			// {
			// 	n_robot_viz.getParam("robot_type",robot_info);
			// 	std::string str= "roslaunch motoman_app load_robot.launch robot_type:="+robot_info;
			// 	const char* command=str.c_str(); 
			// 	system(command);
			// }
			// else if(robot_brand=="abb")
			// {
			// 	n_robot_viz.getParam("robot_type",robot_info);
			// 	std::string str= "roslaunch motoman_app load_robot.launch robot_type:="+robot_info;
			// 	const char* command=str.c_str(); 
			// 	system(command);
			// }
			n_robot_viz.getParam("robot_brand",robot_brand);
			n_robot_viz.getParam("robot_type",robot_info);
			std::string str;
			if(robot_brand != "iiwa")
				str= "roslaunch motoman_app load_robot.launch robot_type:="+robot_info + " robot_brand:="+robot_brand;
			else
				str= "roslaunch iiwa_description "+ robot_name.data+"_upload.launch";
			
			const char* command=str.c_str(); 
			system(command);
		}

    }

};

int main(int argc, char** argv)
{


  	ros::init(argc, argv, "Robot_Viz");
	ROS_INFO_STREAM("Visualizing the robot...");
    
		RobotVisualizer robotObj;    

    while(ros::ok())
    {
    	ros::Rate loop_rate(1000);

  		ros::spinOnce();
  		loop_rate.sleep();
    }  
return 0;
}