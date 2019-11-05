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








int main(int argc, char **argv)
{	
	ROS_INFO("Starting Node robot_trajectory_node");
	ros::init(argc, argv, "traj_no_limit");


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
	string package_name="motoman_app";
	string package_path=ros::package::getPath(package_name);
	string init_file_path=package_path+"/joint_data/init_value_data.csv";
	string jnt_file_path=package_path+"/joint_data/joint_value_gp8_data.csv";
	// string jnt_file_path=package_path+"/joint_data/gp8_traj.csv";
	const char* init_filename =init_file_path.c_str();
	float inti_data[6];
    unsigned int point_num;
    char comma;
	const char* path_filename =jnt_file_path.c_str();
	float data[6];


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
	ROS_INFO("data number: %d", point_num+2);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	if ((point_num+2)<=200)
	 	new_joint_trajectory.points.resize(point_num+2);
	else
		new_joint_trajectory.points.resize(200);
	cout << new_joint_trajectory.points.size();
	int flag=0;
	int counter=0;
	float time=0.0;
	// int adder=0;



	trajectory_msgs::JointTrajectoryPoint next_joint_values;
	next_joint_values.positions.resize(6);
	next_joint_values.velocities.resize(6);
	while(counter<point_num+2)
	{
		ROS_INFO("Start");
		if(flag<(point_num+2)/200)
			new_joint_trajectory.points.resize(200);
		else{
			// new_joint_trajectory.points.resize(point_num+2-((point_num+2)/100)*100+1);
			break;
		}

		for(int i=0; i<new_joint_trajectory.points.size();i++)
		{

			if(i==0)
			{
			
				
				inputFile.open(init_filename);
				if(inputFile.fail())
					{
						ROS_INFO("Error Loading input File");
						
					}
				if(inputFile.is_open())
						ROS_INFO("input file open");


				while(!inputFile.eof()){
						for(int j=0; j<6; j++)
						{
							
							inputFile>>inti_data[j];
							if(j!=5)
								inputFile>>comma;
							next_joint_values.positions[j] = inti_data[j];
							next_joint_values.velocities[j]=0;
							
						}
					}
				inputFile.close();
			
				next_joint_values.time_from_start = ros::Duration(time);
				new_joint_trajectory.points[i] = next_joint_values;
				counter++;
				ROS_INFO("init_data stored");

			}
			else if (i==1 && flag==0)
			{
				for(int j=0; j<6; j++)
				{
							
					// ROS_INFO("data incoming");
					if(j==3)
						next_joint_values.positions[j] = 3.14159;
					else if(j==5)
						next_joint_values.positions[j] = 6.2444;
					else
						next_joint_values.positions[j] = 0.0;
					
					next_joint_values.velocities[j]=0;
					

					// ROS_INFO("data stored");			
				}
				time+=2.5;
				next_joint_values.time_from_start = ros::Duration(time);
				new_joint_trajectory.points[i] = next_joint_values;
				counter++;
				ROS_INFO("home data stored");
			}
			else
			{
				for(int j=0; j<6;j++)
				{
						// ROS_INFO("data incoming");
						jointFile>>data[j];
						if(j!=5)
							jointFile>>comma;
						// ROS_INFO("data recieved");
						next_joint_values.positions[j] = data[j];
						next_joint_values.velocities[j]=0;
						// ROS_INFO("data stored");
						// ROS_INFO("file msg: %f", data[i]);
				}
				counter++;
				if(i==2 && flag==0)
					time+=2.0;
				else
					time+=0.1;
				next_joint_values.time_from_start = ros::Duration(time);
				new_joint_trajectory.points[i] = next_joint_values;
				// ROS_INFO("file value %d stored", counter-2);
			}
			ROS_INFO("EXECUTED: %d, Time: %f", counter, time);

			if(counter >=point_num+2)
			{
				counter+=1;
				break;
			}
		}//for
		curr_goal.trajectory = new_joint_trajectory;
		ac.sendGoal(curr_goal);
		ROS_INFO("data_sent");
		//wait for the action to return
		bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));
		// bool finished_before_timeout= true;
		if (finished_before_timeout)
		{
			actionlib::SimpleClientGoalState state = ac.getState();
		    ROS_INFO("Action finished: %s",state.toString().c_str());
			ROS_INFO("action_Finished ");
			ros::Duration(1).sleep();
			system("rosnode kill init_value_sub_gp8");
			ros::Duration(2).sleep();
		}
		else
		{
		    ROS_INFO("Action did not finish before the time out.");
		    break;
		}
		flag++;
		counter-=1;
		time=0;
		// system("rosrun motoman_app init_value_sub_gp8 ");
	}//while
	
	// trajectory_msgs::JointTrajectoryPoint next_joint_values;
	// int flag =0;
	// ///read in joint values//////////
	// for(int j=0; j<point_num+2;j++)
	// {
	// 	next_joint_values.positions.resize(6);
	// 	next_joint_values.velocities.resize(6);
	// 	if(j==0 && flag ==0)//initial value has to be current trajectory
	// 	{
	// 	    inputFile.open(init_filename);
	// 		if(inputFile.fail())
	// 			{
	// 				ROS_INFO("Error Loading input File");
					
	// 			}
	// 		if(inputFile.is_open())
	// 				ROS_INFO("input file open");


	// 		while(!inputFile.eof()){
	// 				for(int i=0; i<6; i++)
	// 				{
						
	// 					ROS_INFO("data incoming");
	// 					inputFile>>inti_data[i];
	// 					if(i!=5)
	// 						inputFile>>comma;
	// 					ROS_INFO("data recieved");
	// 					next_joint_values.positions[i] = inti_data[i];
	// 					next_joint_values.velocities[i]=0;
						
	// 					ROS_INFO("data stored");
	// 					ROS_INFO("file msg: %f", inti_data[i]);
						
	// 				}
	// 			}

	// 		inputFile.close();
	// 	}
	// 	else if(j==1 && flag ==0)
	// 	{
	// 		for(int i=0; i<6; i++)
	// 		{
						
	// 			ROS_INFO("data incoming");
	// 			next_joint_values.positions[i] = 0.0;
	// 			next_joint_values.velocities[i]=0;
				
	// 			ROS_INFO("data stored");
						
	// 		}
	// 	}

	// 	else 
	// 	{
	// 		for(int i=0; i<6;i++)
	// 		{
	// 				ROS_INFO("data incoming");
	// 				jointFile>>data[i];
	// 				if(i!=5)
	// 					jointFile>>comma;
	// 				ROS_INFO("data recieved");
	// 				next_joint_values.positions[i] = data[i];
	// 				next_joint_values.velocities[i]=0;
					
	// 				ROS_INFO("data stored");
	// 				ROS_INFO("file msg: %f", data[i]);
	// 		}
	// 	}

	// 	ROS_INFO("position %d stored", j);
	// 	if(j==1 && flag ==0)
	// 	{
	// 		time += 2.5;

	// 	}

	// 	else 
	// 	{
			
	// 		if(break_pts_num!=0)
	// 		{
	// 			temp_break=break_pts[k]+3;

				
	// 			if(j==temp_break){
	// 					time+=2.5;
						
	// 					recieve=1;
	// 					k++;
	// 					ROS_INFO("break_encounter");
	// 			}
	// 			if(recieve==0)
	// 			{
	// 				if(j==2 )
	// 				{
	// 					time+=2.5;
						
	// 				}
	// 				else if(j==3)
	// 					time+=2.;
	// 				else if(j==temp_break+1 || j==temp_break+2)
	// 					time+=2.;
	// 				else
	// 					time+=0.2;
	// 			}

	// 			recieve=0;
	// 		}


	// 		else 
	// 		{
	// 			if(j==2)
	// 			{
	// 				time+=2;
					
	// 			}
	// 			else if(j==3)
	// 				time+=0.1;
	// 			else
	// 				time+=0.1;
				
	// 		}

	// 	}
			
	// 	next_joint_values.time_from_start = ros::Duration(time);
	// 	ROS_INFO_ONCE("time: %f", time);
	// 	new_joint_trajectory.points[j] = next_joint_values;

	// 	if(j==99)
	// 	{
	// 		flag =1;
	// 		next_joint_values.resize(100);
	// 		curr_goal.trajectory = new_joint_trajectory;

	// 	}
	// }



	// curr_goal.trajectory = new_joint_trajectory;

	// ac.sendGoal(curr_goal);

	//wait for the action to return
	// bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));

	// if (finished_before_timeout)
	// {
	// 	actionlib::SimpleClientGoalState state = ac.getState();
	//     ROS_INFO("Action finished: %s",state.toString().c_str());
	// }
	// else
	// {
	//     ROS_INFO("Action did not finish before the time out.");
	// }

  	return 0;
}