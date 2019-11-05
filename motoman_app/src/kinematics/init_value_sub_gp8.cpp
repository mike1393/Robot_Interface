//   Author: Yi-Wei, Chen
//   Email: yiweiche@usc.edu
//   ------------------------------------
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include "trajectory_msgs/JointTrajectory.h"
#include <std_msgs/Float64.h>
#include <std_srvs/Trigger.h>
#include <stdio.h>
#include <string>
#include <std_msgs/String.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ros/package.h>
#include <iomanip>



using namespace std;




sensor_msgs::JointState init_msg;
ros::Subscriber sub;
ros::Publisher pub;
void joint_states_callback( const sensor_msgs::JointState::ConstPtr &msg){

   ofstream outFile;
   string package_name="motoman_app";
   string package_path=ros::package::getPath(package_name);
   string out_file_path=package_path+"/joint_data/init_value_data.csv";
	const char* outfilename=out_file_path.c_str();

	float data[6];
	std_msgs::String coor_msg;

	outFile.open(outfilename);
if(outFile.fail())
	{
		ROS_INFO("Error Loading output File");
				
	}
if(outFile.is_open())
		ROS_INFO("output file open"); 
if(sub.getNumPublishers()!=0)
{
	for(int i=0; i<6; i++)
   		init_msg.name[i]=msg->name[i];
   for(int i=0; i<6;i++)
   		init_msg.position[i]=msg->position[i];
    printf("In1[%f,%f,%f,%f,%f,%f] \n",msg->position[0],msg->position[1],msg->position[2],msg->position[3],msg->position[4],msg->position[5]);
    printf("In2[%f,%f,%f,%f,%f,%f] \n",init_msg.position[0],init_msg.position[1],init_msg.position[2],init_msg.position[3],init_msg.position[4],init_msg.position[5]);
   
   if(outFile.is_open())
   {
	   	for(int i=0;i<6;i++)
	   	{
	   		data[i]=msg->position[i];
	   		outFile<<data[i];
	   		if(i!=5)
	   			outFile<< ",";
	   	}
   }

   outFile.close();
 	ROS_INFO("Initial trajectory recieved");


    coor_msg.data = "Service Enabled...\n Please Check if the green light is ON\n";
    pub.publish(coor_msg);
 	sub.shutdown();

}



}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "initial_value_reciever_gp8");
	ros::NodeHandle nc;
	ros::ServiceClient client = nc.serviceClient<std_srvs::Trigger>("robot_enable");
	sleep(1);
	std_srvs::Trigger srv;
	if(client.call(srv))
		ROS_INFO_ONCE("robot_enabled");
	else
		ROS_ERROR("robot_enable failed");
	init_msg.name.resize(6);
	init_msg.name[0]="joint_1_s";
	init_msg.name[1]="joint_2_l";
	init_msg.name[2]="joint_3_u";
	init_msg.name[3]="joint_4_r";
	init_msg.name[4]="joint_5_b";
	init_msg.name[5]="joint_6_t";
	init_msg.position.resize(6);
	init_msg.velocity.resize(6);
 printf("Before:[%f,%f,%f,%f,%f,%f \n]",init_msg.position[0],init_msg.position[1],init_msg.position[2],init_msg.position[3],init_msg.position[4],init_msg.position[5]);


	ros::NodeHandle ns;

	pub = ns.advertise<std_msgs::String>("coodinator_msgs_to_gui", 1000);
	sub=ns.subscribe("/joint_states",1,joint_states_callback);

	 printf("After:[%f,%f,%f,%f,%f,%f \n]",init_msg.position[0],init_msg.position[1],init_msg.position[2],init_msg.position[3],init_msg.position[4],init_msg.position[5]);

	ros::spin();
	return 0;
}
