// Author: Yi-Wei, Chen
// Email: yiweiche@usc.edu
// ------------------------------------


// %Tag(fullSource)%
#include <ros/ros.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <tf2_ros/transform_listener.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/TransformStamped.h>
#include <ros/package.h>

using namespace std;
int main(int argc, char** argv)
{
  ros::init(argc, argv, "import_stand");
  ros::NodeHandle n;
  ros::Rate r(10);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("stand_mesh", 1);
  string package_name="object_viz";
  string package_path=ros::package::getPath(package_name);
  string file_path="file://"+package_path+"/src/STL/Stand.STL";



  // bool cal=false;

  // create  marker object
  while (ros::ok())
  {
	visualization_msgs::Marker marker;
	marker.header.frame_id = "base_link";
	marker.header.stamp = ros::Time::now();
	
	marker.ns="import_stand";
	marker.id=0;
	marker.type = visualization_msgs::Marker::MESH_RESOURCE;
	marker.mesh_resource = file_path;
	marker.action = visualization_msgs::Marker::ADD;
	
	marker.scale.x = 0.001;
	marker.scale.y = 0.001;
	marker.scale.z = 0.001;
	marker.color.r = 1.0;
	marker.color.g = 1.0;
	marker.color.b = 0.0;
	marker.color.a = 1.0;
	marker.pose.position.x = 0.33+0.075;
	marker.pose.position.y = 0.075-0.0508;
	marker.pose.position.z = 0.0;
	//create tf listener to recieve pose data of tool0 link

	//   //create point marker
	//   visualization_msgs::Marker pts;
	//   pts.type = visualization_msgs::Marker::POINTS;  
	//   pts.scale.x = 0.01;
	//   pts.scale.y = 0.01;
	//   pts.color.r = 1.0;
	//   pts.color.g = 0.0;
	//   pts.color.b = 0.0;
	//   pts.color.a = 1.0;
	// geometry_msgs::Point p;
	
	// while(marker_pub.getNumSubscribers() < 1)
	// {
	// 	if (!ros::ok())
	// 	{
	// 		return 0;
	// 	}
	// 	if(!cal)
	// 	{
	// 		inputFile.open(filename);
	// 		ROS_INFO(filename);
	// 		if(inputFile.fail())
	// 		{
	// 			ROS_INFO("Error Loading input File");
	// 			return 1;
	// 		}
	// 		if(inputFile.is_open())
	// 			ROS_INFO("input file open");  
			
			
	// 		//save tool base position
	// 		outFile.open(outfilename);
	// 		ROS_INFO(outfilename);
	// 		if(outFile.fail())
	// 		{
	// 			ROS_INFO("Error Loading output File");
	// 			return 1;
	// 		}
	// 		if(outFile.is_open())
	// 			ROS_INFO("output file open"); 
			
	// 		while(!inputFile.eof())
	// 		{
	// 			for(int i=0;i<12;i++)
	// 		  {
	// 			  inputFile>>data[i];
	// 			  if(i!=11)
	// 				inputFile>>comma;
	// 			  if(i==11)
	// 			  {
	// 				  	for(int j=0; j<12;j++)
	// 				  	{
	// 				  		if(j ==0 )//x value
	// 					  {
	// 						  outFile<<data[j]-tool_len*data[9];
	// 						  //p.x=(data[j]-tool_len*data[9])/1000.;
	// 					  }
							  
	// 					  else if (j==1)// y value
	// 					  {
	// 						   outFile<<data[j]-tool_len*data[10];
	// 						   //p.y=(data[j]-tool_len*data[10])/1000.;
	// 					  }
							 
	// 					  else if(j==2)// z value
	// 					  {
	// 						   outFile<<data[j]-tool_len*data[11];
	// 						   //p.z=(data[j]-tool_len*data[11])/1000.;
	// 					  }
	// 					  else if(j>2)// orientation values
	// 						  outFile<<(data[j]);
						  
	// 					  if(j!=11)
	// 						  outFile<<",";
	// 					  else if(j==11)
	// 						  outFile<<"\n";
	// 				   }
	// 				   //pts.points.push_back(p);
	// 			  	}


					  
	// 		  }
			  
	// 		}
	// 		outFile.close();

	// 		ROS_INFO("output file closed");  
	// 		ROS_INFO("output file generated");  
	// 		cal=true;
	// 	}
	// 	ROS_WARN_ONCE("Please create a subscriber to the marker");
	// 	sleep(1);
	// }
	
	
	while(marker_pub.getNumSubscribers() > 0)
	{
		// while tf listener does not recieve anything 



		// // while tf listener recieves something
		// try{
		// 	geometry_msgs::TransformStamped transformStamped;
		// 	transformStamped = tfBuffer.lookupTransform("base_link","tool0",ros::Time(0));//target fram, source frame, ros::Time(0)
		// 	marker.pose.position.x=transformStamped.transform.translation.x;
		// 	marker.pose.position.y=transformStamped.transform.translation.y;
		// 	marker.pose.position.z=transformStamped.transform.translation.z;
		// 	marker.pose.orientation.x =transformStamped.transform.rotation.x;
		// 	marker.pose.orientation.y =transformStamped.transform.rotation.y;
		// 	marker.pose.orientation.z =transformStamped.transform.rotation.z;
		// 	marker.pose.orientation.w =transformStamped.transform.rotation.w;
		// 	ROS_INFO_ONCE("tool connected...");
		// 	// printf("marker pose:  %f, %f, %f \n", marker.pose.position.x, marker.pose.position.y,marker.pose.position.z);
		// 	// printf("marker rot:  %f, %f, %f ,%f \n", marker.pose.orientation.x, marker.pose.orientation.y,marker.pose.orientation.z,marker.pose.orientation.w);
		// }
		// catch (tf2::TransformException &ex)
		// {

		// 	ROS_WARN("%s",ex.what());
		// 	//ros::Duration(1.0).sleep();

		// }
		
		

		marker.lifetime = ros::Duration();

		marker_pub.publish(marker);
		//marker_pub.publish(pts);
		
		r.sleep();
		  
	}
  }

	return 0;
}
// %Tag(fullSource)%
