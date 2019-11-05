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

class Robot_Enable_Call
{
public:

	ros::NodeHandle n_robot_enable_viz;
	ros::Subscriber robot_enable_name_sub;
	ros::Publisher robot_enable_pub;
	rviz_visual_tools::RvizVisualToolsPtr visual_tools_;
	std::string robot_info;
	std::string robot_ip;
	std::string robot_ctrl;

	Robot_Enable_Call()
	{
		robot_enable_name_sub 
        = n_robot_enable_viz.subscribe("robot_enable_pub", 1000, &Robot_Enable_Call::callService, this);



        robot_enable_pub=n_robot_enable_viz.advertise<std_msgs::String>("coodinator_msgs_to_gui",1000);
	}
	~Robot_Enable_Call()
	{
		robot_enable_name_sub.shutdown();

	}
	void callService(const std_msgs::String& enable_msg)
	{

		if (enable_msg.data.compare("robot_enable")==0)
		{
			
			std_msgs::String msg;
            msg.data = "Calling Service ...\n Make Sure the Emergency Stop is OFF\n";
            robot_enable_pub.publish(msg);

			ROS_INFO_ONCE("Calling Service ...");
			n_robot_enable_viz.getParam("robot_type",robot_info);
			n_robot_enable_viz.getParam("robot_ip",robot_ip);
			n_robot_enable_viz.getParam("controller",robot_ctrl);
			n_robot_enable_viz.setParam("robot_enable","yes");
			std::string str= "rosnode kill robot_state_publisher && roslaunch motoman_app robot_interface.launch robot_type:="
			+ robot_info+" controller:="+robot_ctrl+" robot_ip:="+robot_ip; 
			const char* command=str.c_str(); 
			system(command);
		}

    }


};

int main(int argc, char** argv)
{


  	ros::init(argc, argv, "enable_Viz");
	ROS_INFO_STREAM("Opening seperate window...");
    
		Robot_Enable_Call simObj;    

    while(ros::ok())
    {
    	ros::Rate loop_rate(1000);

  		ros::spinOnce();
  		loop_rate.sleep();
    }  
return 0;
}