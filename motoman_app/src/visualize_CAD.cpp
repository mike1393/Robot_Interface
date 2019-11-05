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

class CAD_Visualizer
{
public:

	ros::NodeHandle n_cad_viz;
	ros::Subscriber cad_name_sub;
	ros::Publisher cad_model_pub;
	rviz_visual_tools::RvizVisualToolsPtr visual_tools_;

	std::string urdf_file;

	CAD_Visualizer()
	{
        cad_name_sub = n_cad_viz.subscribe("CAD_input", 1000, &CAD_Visualizer::getCADInfo, this);
	};
	~CAD_Visualizer()
	{
		system("rosnode kill import_file");
		ROS_INFO("shutdown");


	}
	void getCADInfo(const std_msgs::String& cad_name)
	{
		

		if (cad_name.data.compare("openSTL")==0)
		{
			ROS_INFO("start import file");
			system("rosrun object_viz import_file");
		}
		else
		{
			ROS_INFO("Waiting for signal");
		}

    }

};

int main(int argc, char** argv)
{

  	ros::init(argc, argv, "CAD_Viz");
	ROS_INFO_STREAM("Visualizing the CAD...");
    
		CAD_Visualizer robotObj;    

    while(ros::ok())
    {
    	ros::Rate loop_rate(1000);

  		ros::spinOnce();
  		loop_rate.sleep();
    }  
return 0;
}