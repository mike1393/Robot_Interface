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

class ToolVisualizer
{
public:

	ros::NodeHandle n_tool_viz;
	ros::Subscriber tool_name_sub;
	ros::Publisher tool_pub;
	rviz_visual_tools::RvizVisualToolsPtr visual_tools_;
	std::string urdf_file;

	ToolVisualizer()
	{
        tool_name_sub = n_tool_viz.subscribe("tool_viz_pub", 1000, &ToolVisualizer::getToolInfo, this);
	};
	~ToolVisualizer()
	{
		n_tool_viz.deleteParam("/tool_mesh");
		n_tool_viz.shutdown();
	}
	void getToolInfo(const std_msgs::String& tool_name)
	{
		ROS_INFO_STREAM(tool_name.data);

		if (tool_name.data.size()!=0)
		{
			if(tool_name.data.compare("probe")==0)
				n_tool_viz.setParam("tool_len","96");
			else
				n_tool_viz.setParam("tool_len","150");
			std::string package_path = ros::package::getPath("motoman_app");
			urdf_file = package_path + "/urdf/tool/" + tool_name.data + ".xacro";
			std::ifstream f(urdf_file); 
			if(f.fail())
				ROS_ERROR("Error Loading tool File");
			std::stringstream ss;
			ss << f.rdbuf();
			n_tool_viz.setParam("/tool_mesh", ss.str());
			
		}

    }

};

int main(int argc, char** argv)
{


  	ros::init(argc, argv, "Tool_Viz");
	ROS_INFO_STREAM("Visualizing the tool...");
    
		ToolVisualizer toolObj;    

    while(ros::ok())
    {
    	ros::Rate loop_rate(1000);

  		ros::spinOnce();
  		loop_rate.sleep();
    }  
return 0;
}