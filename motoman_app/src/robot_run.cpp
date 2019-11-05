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

class Trajectory_Visualizer
{
public:

	ros::NodeHandle n_run_viz;
	ros::Subscriber run_name_sub;

	ros::Publisher run_pub;
	rviz_visual_tools::RvizVisualToolsPtr visual_tools_;
	std::string robot_info;

	std::string urdf_file;

	Trajectory_Visualizer()
	{
		run_name_sub 
        = n_run_viz.subscribe("robot_run_pub", 1000, &Trajectory_Visualizer::trajectory_run, this);
        run_pub=n_run_viz.advertise<std_msgs::String>("coodinator_msgs_to_gui",1000);
	}
	~Trajectory_Visualizer()
	{
		run_name_sub.shutdown();

	}
	void trajectory_run(const std_msgs::String& run_msg)
	{

		if (run_msg.data.compare("robot_run")==0)
		{
			n_run_viz.getParam("robot_type",robot_info);
			std::string package_name = "motoman_app";
		    // std::string file_name="trajectory_pub_"+robot_info;
		    std::string file_name="traj_no_limit";
			std::string str= "rosrun "+package_name+" "+file_name;
			std_msgs::String msg;
            msg.data = "Robot Streaming Start...\n Please Stand Back...\n";
            run_pub.publish(msg);
			const char* command=str.c_str(); 
			system(command);

			msg.data = "Robot Streaming END\n Please Enable robot for further execution\n";
			run_pub.publish(msg);
			file_name="init_value_sub_"+robot_info;
			str= "rosnode kill "+file_name +" && roslaunch motoman_app load_robot.launch robot_type:="+robot_info;
			command=str.c_str(); 
			system(command);

		}

    }

};

int main(int argc, char** argv)
{


  	ros::init(argc, argv, "run_Viz");
	ROS_INFO_STREAM("Opening seperate window...");
    
		Trajectory_Visualizer simObj;    

    while(ros::ok())
    {
    	ros::Rate loop_rate(1000);

  		ros::spinOnce();
  		loop_rate.sleep();
    }  
return 0;
}