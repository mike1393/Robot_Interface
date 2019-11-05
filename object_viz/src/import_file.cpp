// Author: Yi-Wei, Chen
// Email: yiweiche@usc.edu
// ------------------------------------
// %Tag(fullSource)%
#include <ros/ros.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <interactive_markers/interactive_marker_server.h>
#include <string>
#include "std_msgs/String.h"
#include "geometry_msgs/Quaternion.h"
#include <ros/package.h>

using namespace std;

  visualization_msgs::Marker marker;

  string package_name="object_viz";
  string package_path=ros::package::getPath(package_name);
  string file_path=package_path+"/src/data/CAD_data/ak_points_new.csv";
  string out_file_path=package_path+"/src/data/test_global_value.csv";


  // ros::Publisher pub=n.advertise<std_msgs::String>("coodinator_msgs_to_gui",1000);
  // std_msgs::String msg;

void processFeedback(
    const visualization_msgs::InteractiveMarkerFeedbackConstPtr &feedback )
{
  ROS_INFO_STREAM( feedback->marker_name << " is now at \n"
      << feedback->pose.position.x << ", " << feedback->pose.position.y
      << ", " << feedback->pose.position.z <<"\n"
	  << feedback->pose.orientation.w << ", "
	  << feedback->pose.orientation.x << ", "
	  << feedback->pose.orientation.y << ", "
	  << feedback->pose.orientation.z );

  // msg.data=to_string(feedback->pose.position.x)+to_string(feedback->pose.position.y)+to_string(feedback->pose.position.z);
  // msg.data=(feedback->pose.position.x).
  // pub.publish(msg);
  float q_rot[9];
  const char* filename = file_path.c_str();
const char* outfilename = out_file_path.c_str();
  ifstream inputFile;
  ofstream outFile;
  float data[12];
  float newbee[12];
  char comma;
  int check=0;
  int j=0;
  inputFile.open(filename);
  ROS_INFO_ONCE(filename);
  outFile.open(outfilename);
  ROS_INFO_ONCE(outfilename);
  while(!inputFile.eof())
  {
    for(int i=0;i<12;i++)
    {
      inputFile>>data[i];
      if(i!=11)
      inputFile>>comma;
      if(i==11){
        check=1;
        j=0;
      }
      while(check)
      {



          if(j ==0 )
          {
            outFile<<(data[j]/1000.+feedback->pose.position.x)*1000.;
          }
            
          else if (j==1)
          {

             outFile<<(data[j]/1000.+feedback->pose.position.y)*1000.;
          }
           
          else if(j==2)
          {
             outFile<<(data[j]/1000.+feedback->pose.position.z)*1000.;
          }
          else if(j>2)
            outFile<<(data[j]);
          
          if(j!=11)
            outFile<<",";
          else if(j==11){
            outFile<<"\n";
            check=0;
          }
          j++;
      }

        
    }
  }
  inputFile.close();
  outFile.close();
  ROS_INFO("output file closed in server");  
  ROS_INFO("output file generated in server"); 

}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "import_file");
    // ros::NodeHandle n;

  // create an interactive marker server on the topic namespace simple_marker
  interactive_markers::InteractiveMarkerServer server("import_file");
      std::string package_path = ros::package::getPath("object_viz");
      std::string mesh_file= "file://"+package_path + "/src/STL/myshape_default_resolution.STL";
  server.clear();
  server.applyChanges();
  if(server.empty())
    ROS_INFO("server is empty");
  // create an interactive marker for our server
  visualization_msgs::InteractiveMarker int_marker;
  int_marker.header.frame_id = "base_link";
  int_marker.header.stamp=ros::Time::now();
  int_marker.name = "my_marker";
  int_marker.description = "import object";

  // create  marker object

  marker.lifetime = ros::Duration(0);
  marker.mesh_resource = "";
  marker.header.frame_id = "base_link";
  marker.type = visualization_msgs::Marker::MESH_RESOURCE;
  marker.action = visualization_msgs::Marker::ADD;
 marker.mesh_resource = mesh_file;


  marker.pose.position.x = 0.33;//0.33
  marker.pose.position.y = -0.0508;
  marker.pose.position.z = 0.3175-0.01;//0.3075
  // marker.pose.position.x = 0.401;//0.58
  // marker.pose.position.y = 0.036;
  // marker.pose.position.z = 0.307;//0.33
  marker.pose.orientation.x = 0.0;
  marker.pose.orientation.y = 0.0;
  marker.pose.orientation.z = 0.0;
  marker.pose.orientation.w = 1.0;
  marker.scale.x = 0.001;
  marker.scale.y = 0.001;
  marker.scale.z = 0.001;
  marker.color.r = 0.5;
  marker.color.g = 0.5;
  marker.color.b = 0.5;
  marker.color.a = 1.0;
  marker.lifetime = ros::Duration(1000);

  
  int_marker.pose.position.x=marker.pose.position.x ;
 int_marker.pose.position.y=marker.pose.position.y;
  int_marker.pose.position.z=marker.pose.position.z ;
  
  // create a non-interactive control which contains the box
  visualization_msgs::InteractiveMarkerControl box_control;
  box_control.always_visible = true;
  box_control.markers.push_back( marker );

  // add the control to the interactive marker
  int_marker.controls.push_back( box_control );



  //create point marker
  visualization_msgs::Marker pts;
  pts.type = visualization_msgs::Marker::POINTS;  
  pts.scale.x = 0.01;
  pts.scale.y = 0.01;
  pts.color.r = 0.0;
  pts.color.g = 1.0;
  pts.color.b = 0.0;
  pts.color.a = 1.0;
  
  //read in position file and push the position for points
  ifstream inputFile;
  ofstream outFile;
  // point coordinates describe in local frame
  // format: x,y,z, Rx,Ry,Rz


  const char* filename = file_path.c_str();

  // point coordinate describe in global frame
  // format: x,y,z, Rx,Ry,Rz
  const char* outfilename = out_file_path.c_str();
  float data[12];
  char comma;
  geometry_msgs::Point p;
  
  inputFile.open(filename);
  ROS_INFO(filename);
	if(inputFile.fail())
	{
		ROS_INFO("Error Loading input File");
		return 1;
	}
	if(inputFile.is_open())
		ROS_INFO("input file open");  
	
	
	  //save current position
	outFile.open(outfilename);
	ROS_INFO(outfilename);
	if(outFile.fail())
	{
		ROS_INFO("Error Loading output File");
		return 1;
	}
	if(outFile.is_open())
		ROS_INFO("output file open");  
	
	
  while(!inputFile.eof())
  {
	  for(int i=0;i<12;i++)
	  {
		  inputFile>>data[i];
		  if(i!=11)
			inputFile>>comma;
		  if(i ==0 )
		  {
			  p.x=data[i]/1000.;
			  outFile<<(p.x+marker.pose.position.x)*1000.;
			  
			 
		  }
			  
		  else if (i==1)
		  {
			   p.y=data[i]/1000.;
			   outFile<<(p.y+marker.pose.position.y)*1000.;
		  }
			 
		  else if(i==2)
		  {
			   p.z=data[i]/1000.;
			   outFile<<(p.z+marker.pose.position.z)*1000.;
		  }
		  else if(i>2)
			  outFile<<(data[i]);
		  
		  if(i!=11)
			  outFile<<",";
		  else if(i==11)
			  outFile<<"\n";
			  
	  }
	  pts.points.push_back(p);
  }
  outFile.close();
  ROS_INFO("output file closed");  
  ROS_INFO("output file generated"); 
  
  
  
   //
  // create a non-interactive control which contains the points
  visualization_msgs::InteractiveMarkerControl pts_control;
  pts_control.always_visible = true;
  pts_control.markers.push_back( pts );

  // add the control to the interactive marker
  int_marker.controls.push_back( pts_control );
  //
  
  
  // create a control which will move the box
  visualization_msgs::InteractiveMarkerControl control;

    control.orientation.w = 1;
    control.orientation.x = 1;
    control.orientation.y = 0;
    control.orientation.z = 0;
    control.name = "rotate_x";
    control.interaction_mode = visualization_msgs::InteractiveMarkerControl::ROTATE_AXIS;
    int_marker.controls.push_back(control);
    control.name = "move_x";
    control.interaction_mode = visualization_msgs::InteractiveMarkerControl::MOVE_AXIS;
    int_marker.controls.push_back(control);

    control.orientation.w = 1;
    control.orientation.x = 0;
    control.orientation.y = 1;
    control.orientation.z = 0;
    control.name = "rotate_z";
    control.interaction_mode = visualization_msgs::InteractiveMarkerControl::ROTATE_AXIS;
    int_marker.controls.push_back(control);
    control.name = "move_z";
    control.interaction_mode = visualization_msgs::InteractiveMarkerControl::MOVE_AXIS;
    int_marker.controls.push_back(control);

    control.orientation.w = 1;
    control.orientation.x = 0;
    control.orientation.y = 0;
    control.orientation.z = 1;
    control.name = "rotate_y";
    control.interaction_mode = visualization_msgs::InteractiveMarkerControl::ROTATE_AXIS;
    int_marker.controls.push_back(control);
    control.name = "move_y";
    control.interaction_mode = visualization_msgs::InteractiveMarkerControl::MOVE_AXIS;
    int_marker.controls.push_back(control);

  // add the interactive marker to our collection &
  // tell the server to call processFeedback() when feedback arrives for it
  server.insert(int_marker, &processFeedback);
  //server.setCallback(int_marker.name, &processFeedback);

  // 'commit' changes and send to all clients
  server.applyChanges();
  // box_control.markers.push_back( marker );

  // // add the control to the interactive marker
  // int_marker.controls.push_back( box_control );

  // pts_control.markers.push_back( pts );

  // // add the control to the interactive marker
  // int_marker.controls.push_back( pts_control );
  // ROS_INFO("Hi");
  //start the ROS main loop
  while(ros::ok())
  {
      ros::Rate loop_rate(1000);
      ros::spinOnce();
      
  }

  ROS_INFO("OOPS");
  return 0;
}
// %Tag(fullSource)%
