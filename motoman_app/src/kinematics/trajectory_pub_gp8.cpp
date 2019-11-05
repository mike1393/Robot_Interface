//   Author: Yi-Wei, Chen
//   Email: yiweiche@usc.edu
//   ------------------------------------
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/JointState.h"
#include "trajectory_msgs/JointTrajectory.h"
#include "control_msgs/FollowJointTrajectoryAction.h"
#include "actionlib/client/simple_action_client.h"
#include <actionlib/client/terminal_state.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ros/package.h>

using namespace std;






void joint_values()
{
   
}

int main(int argc, char **argv)
{	
	ROS_INFO("Starting Node robot_trajectory_node");
	ros::init(argc, argv, "trajectory_pub_gp8");


	actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction> ac("/joint_trajectory_action");
	ROS_INFO("Waiting for action server to start.");
	ac.waitForServer(); //will wait for infinite time
	ROS_INFO("Action server started, sending goal.");

	control_msgs::FollowJointTrajectoryGoal curr_goal;
	

    


      trajectory_msgs::JointTrajectory new_joint_trajectory;

	new_joint_trajectory.header.stamp = ros::Time::now();
	new_joint_trajectory.header.frame_id = "temp_pose";
	new_joint_trajectory.joint_names.resize(6);
	

	new_joint_trajectory.joint_names[0] = "joint_1_s";
	new_joint_trajectory.joint_names[1] = "joint_2_l";
	new_joint_trajectory.joint_names[2] = "joint_3_u";
	new_joint_trajectory.joint_names[3] = "joint_4_r";
	new_joint_trajectory.joint_names[4] = "joint_5_b";
	new_joint_trajectory.joint_names[5] = "joint_6_t";
	//read in the initial value.csv file
	ifstream inputFile;
	ifstream jointFile;
	ifstream breakFile;
	string package_name="motoman_app";
	string package_path=ros::package::getPath(package_name);
	string init_file_path=package_path+"/joint_data/init_value_data.csv";
	// string jnt_file_path=package_path+"/joint_data/joint_value_gp8_data.csv";
	string jnt_file_path=package_path+"/joint_data/gp8_traj.csv";
	string brk_file_path=package_path+"/joint_data/break_point_data.csv";
	const char* init_filename =init_file_path.c_str();
	float inti_data[6];
    unsigned int point_num;
    char comma;
	const char* path_filename =jnt_file_path.c_str();
	float data[6];
    const char* break_filename = brk_file_path.c_str();
    int break_pts_num;
    int *break_pts;
    float prep_time=2.5;//from current pts to start pts
    float start_time=prep_time+0.5;//from start_pts to 1st pts
    int recieve=0;
    float time=0.0;
    int temp_break;
    int k=0;
    ///store break point values///////////
    breakFile.open(break_filename);
    if(breakFile.fail())
		{
			ROS_INFO("Error Loading break File");
			
		}
	if(breakFile.is_open())
			ROS_INFO("break file open");
	breakFile>>comma;
	breakFile>>break_pts_num;
	ROS_INFO("break number: %d", break_pts_num);
	if(break_pts_num!=0)
	{
		break_pts = new int[break_pts_num];
		for(int i=0; i<break_pts_num;i++){
				breakFile>>break_pts[i];
				ROS_INFO("break_pts: %d", break_pts[i]);
				if(i!=break_pts_num-1)
					breakFile>>comma;
			}
	}

    ////joint value preparation////
    jointFile.open(path_filename);
    if(jointFile.fail())
		{
			ROS_INFO("Error Loading joint File");
			
		}
	if(jointFile.is_open())
			ROS_INFO("joint file open");
	jointFile>>comma;
	jointFile>>point_num;
	ROS_INFO("data number: %d", point_num);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	if ((point_num+2)<=100)
	 	new_joint_trajectory.points.resize(point_num+2);
	else
		new_joint_trajectory.points.resize(100);

	trajectory_msgs::JointTrajectoryPoint next_joint_values;
	int flag =0;
	///read in joint values//////////
	for(int j=0; j<point_num+2;j++)
	{
		next_joint_values.positions.resize(6);
		next_joint_values.velocities.resize(6);
		if(j==0 && flag ==0)//initial value has to be current trajectory
		{
		    inputFile.open(init_filename);
			if(inputFile.fail())
				{
					ROS_INFO("Error Loading input File");
					
				}
			if(inputFile.is_open())
					ROS_INFO("input file open");


			while(!inputFile.eof()){
					for(int i=0; i<6; i++)
					{
						
						ROS_INFO("data incoming");
						inputFile>>inti_data[i];
						if(i!=5)
							inputFile>>comma;
						ROS_INFO("data recieved");
						next_joint_values.positions[i] = inti_data[i];
						next_joint_values.velocities[i]=0;
						
						ROS_INFO("data stored");
						ROS_INFO("file msg: %f", inti_data[i]);
						
					}
				}

			inputFile.close();
		}
		else if(j==1 && flag ==0)
		{
			for(int i=0; i<6; i++)
			{
						
				ROS_INFO("data incoming");
				next_joint_values.positions[i] = 0.0;
				next_joint_values.velocities[i]=0;
				
				ROS_INFO("data stored");
						
			}
		}

		else 
		{
			for(int i=0; i<6;i++)
			{
					ROS_INFO("data incoming");
					jointFile>>data[i];
					if(i!=5)
						jointFile>>comma;
					ROS_INFO("data recieved");
					next_joint_values.positions[i] = data[i];
					next_joint_values.velocities[i]=0;
					
					ROS_INFO("data stored");
					ROS_INFO("file msg: %f", data[i]);
			}
		}

		ROS_INFO("position %d stored", j);
		if(j==1 && flag ==0)
		{
			time += 2.5;

		}

		else 
		{
			
			if(break_pts_num!=0)
			{
				temp_break=break_pts[k]+3;

				
				if(j==temp_break){
						time+=2.5;
						
						recieve=1;
						k++;
						ROS_INFO("break_encounter");
				}
				if(recieve==0)
				{
					if(j==2 )
					{
						time+=2.5;
						
					}
					else if(j==3)
						time+=2.;
					else if(j==temp_break+1 || j==temp_break+2)
						time+=2.;
					else
						time+=0.2;
				}

				recieve=0;
			}


			else 
			{
				if(j==2)
				{
					time+=2;
					
				}
				else if(j==3)
					time+=0.1;
				else
					time+=0.1;
				
			}

		}
			
		next_joint_values.time_from_start = ros::Duration(time);
		ROS_INFO_ONCE("time: %f", time);
		new_joint_trajectory.points[j] = next_joint_values;

		// if(j==99)
		// {
		// 	flag =1;
		// 	// next_joint_values.resize(100);
		// 	curr_goal.trajectory = new_joint_trajectory;

		// }
	}



	curr_goal.trajectory = new_joint_trajectory;

	ac.sendGoal(curr_goal);

	//wait for the action to return
	bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));

	if (finished_before_timeout)
	{
		actionlib::SimpleClientGoalState state = ac.getState();
	    ROS_INFO("Action finished: %s",state.toString().c_str());
	}
	else
	{
	    ROS_INFO("Action did not finish before the time out.");
	}

  	return 0;
}