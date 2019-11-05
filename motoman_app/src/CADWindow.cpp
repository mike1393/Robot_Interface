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

class CADWin_Visualizer
{
public:

	ros::NodeHandle n_CAD_viz;
	ros::Subscriber CAD_name_sub;
	ros::Publisher CAD_pub;
	rviz_visual_tools::RvizVisualToolsPtr visual_tools_;
	std_msgs::String CAD_trigger;
	std::string urdf_file;
	std::string status;

	CADWin_Visualizer()
	{
		CAD_trigger.data="none";
		n_CAD_viz.setParam("Software_status","close");
		n_CAD_viz.getParam("Software_status",status);
		CAD_pub=n_CAD_viz.advertise<std_msgs::String>("CAD_input",1000);
        CAD_name_sub = n_CAD_viz.subscribe("CADSoft_pub", 1000, &CADWin_Visualizer::getCADwindow, this);

	};
	~CADWin_Visualizer()
	{
		CAD_pub.shutdown();
		CAD_name_sub.shutdown();
	}
	void getCADwindow(const std_msgs::String& CAD_name)
	{

		if (CAD_name.data.compare("start")==0)
		{
			n_CAD_viz.getParam("Software_status",status);
			std::string package_name = "CAD_UI";
			std::string package_path = ros::package::getPath(package_name);
			std::string file_path = package_path + "/src/new_ui";

			if(status=="open")
			{

				n_CAD_viz.setParam("Software_status","close");
				n_CAD_viz.getParam("Software_status",status);
				system("rosnode kill import_file");
				system("rosnode kill rviz");

				sleep(1);
			}


			if(status == "close")
			{
				std::string str="cd "+file_path+" && python3.6 MainProgram.py";
				const char* command = str.c_str();
				n_CAD_viz.setParam("Software_status","open");
				system(command);
				ROS_INFO("Window closed...");

			}

			CAD_trigger.data="openSTL";
			CAD_pub.publish(CAD_trigger);
			ROS_INFO("trigger sent...");
		}

    }

};

int main(int argc, char** argv)
{

  	ros::init(argc, argv, "CAD_win");
	ROS_INFO_STREAM("Opening seperate window...");
    
		CADWin_Visualizer CADObj;    

    while(ros::ok())
    {
    	ros::Rate loop_rate(1000);

  		ros::spinOnce();
  		loop_rate.sleep();
    }  
return 0;
}