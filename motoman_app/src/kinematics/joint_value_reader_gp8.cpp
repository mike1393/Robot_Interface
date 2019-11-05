//   Author: Yi-Wei, Chen
//   Email: yiweiche@usc.edu
//   ------------------------------------
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <std_msgs/Float64.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <ros/package.h>



using namespace std;
int main(int argc, char **argv)
{
	ros::init(argc, argv, "joint_value_reader_gp8");
	ros::NodeHandle n;
	ros::Publisher pub= n.advertise<sensor_msgs::JointState>("input_value", 1000);
	ros::Rate loop_rate(10);
	while(pub.getNumSubscribers() == 0)
	{
		ROS_WARN_ONCE("Expecting Subscriber...");
			ros::spinOnce();
			loop_rate.sleep();
		if(!ros::ok())
		{
			return 0;
		}
		
	}
	while(pub.getNumSubscribers() != 0 && ros::ok())
	{

		float data[6];
		char comma;
		int num;
		sensor_msgs::JointState msg;
		msg.name.resize(6);
		
		msg.name[0]="joint_1_s";
		msg.name[1]="joint_2_l";
		msg.name[2]="joint_3_u";
		msg.name[3]="joint_4_r";
		msg.name[4]="joint_5_b";
		msg.name[5]="joint_6_t";
		msg.position.resize(6);
		ifstream inputFile;
		//file path
		string package_name="motoman_app";
		string package_path=ros::package::getPath(package_name);
		string file_path=package_path+"/joint_data/joint_value_gp8_data.csv";
		// string file_path=package_path+"/joint_data/gp8_traj.csv";
		
		const char* filename = file_path.c_str();
		ROS_INFO(filename);
		inputFile.open(filename);
		if(inputFile.fail())
		{
			ROS_INFO("Error Loading File");
			return 1;
		}
		if(inputFile.is_open())
			ROS_INFO("file open");
		inputFile>>comma;
		inputFile>>num;


		while(!inputFile.eof() && ros::ok()){
			for(int i=0; i<6; i++)
			{
				
				
				inputFile>>data[i];
				if(i!=5)
					inputFile>>comma;
				
				msg.position[i]=data[i];

			}
			
			pub.publish(msg);
			
			ros::spinOnce();
			loop_rate.sleep();			
		}
		ROS_INFO("file end");
		break;
	}
	return 0;
}
