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

class SimulationWin_Visualizer
{
public:

	ros::NodeHandle n_sim_viz;
	ros::Subscriber sim_name_sub;
	ros::Subscriber robot_sub;
	ros::Publisher sim_pub;
	rviz_visual_tools::RvizVisualToolsPtr visual_tools_;
	std::string robot_info;

	std::string urdf_file;

	SimulationWin_Visualizer()
	{
		sim_name_sub 
        = n_sim_viz.subscribe("simulation_pub", 1000, &SimulationWin_Visualizer::getSIMwindow, this);
        sim_pub=n_sim_viz.advertise<std_msgs::String>("coodinator_msgs_to_gui",1000);
	}
	~SimulationWin_Visualizer()
	{
		sim_name_sub.shutdown();

	}
	void getSIMwindow(const std_msgs::String& sim_name)
	{

		if (sim_name.data.compare("start_sim")==0)
		{
			n_sim_viz.getParam("robot_type",robot_info);
			std::string package_name = "motoman_app";
		    std::string file_name="joint_value_reader_"+robot_info;
			std::string str= "cd /home/cam_sanding/catkin_ws/src/stepUI_py35/ && python3 Generate_Transformation_Matrix.py && rosrun "+package_name+" "+file_name;
			std_msgs::String msg;
            msg.data = "Simulation Start...\n";
            ROS_INFO("start");
            sim_pub.publish(msg);
			const char* command=str.c_str(); 
			// system("rosnode kill /joint_value_reader_gp8");

			system(command);
			msg.data = "Simulation END...\n";
			sim_pub.publish(msg);

		}

    }

};

int main(int argc, char** argv)
{


  	ros::init(argc, argv, "sim_Viz");
	ROS_INFO_STREAM("Opening seperate window...");
    
		SimulationWin_Visualizer simObj;    

    while(ros::ok())
    {
    	ros::Rate loop_rate(1000);

  		ros::spinOnce();
  		loop_rate.sleep();
    }  
return 0;
}