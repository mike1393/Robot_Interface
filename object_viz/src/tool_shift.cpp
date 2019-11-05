// Author: Yi-Wei, Chen
// Email: yiweiche@usc.edu
// ------------------------------------


// %Tag(fullSource)%
#include <ros/ros.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ros/package.h>


using namespace std;
int main(int argc, char** argv)
{
  ros::init(argc, argv, "import_tool");



	ifstream inputFile;
	ofstream outFile;
	string package_name="object_viz";
	string package_path=ros::package::getPath(package_name);
	string file_path=package_path+"/src/data/test_global_value.csv";
	// string file_path=package_path+"/src/data/CAD_data/ak_points_new.csv";
	string out_file_path=package_path+"/src/data/tool_base_value.csv";
	const char* filename =file_path.c_str();
	const char* outfilename=out_file_path.c_str();
	ros::NodeHandle n_tool_len;
	float data[12];
	char comma;
	string tool_str;
	n_tool_len.getParam("tool_len",tool_str);
	float tool_len=atof(tool_str.c_str());
	

		if (!ros::ok())
		{
			return 0;
		}
		else 
		{
			inputFile.open(filename);
			ROS_INFO(filename);
			if(inputFile.fail())
			{
				ROS_WARN("Error Loading input File");
				return 1;
			}
			if(inputFile.is_open())
				ROS_INFO("input file open");  

			//save tool base position
			outFile.open(outfilename);
			ROS_INFO(outfilename);
			if(outFile.fail())
			{
				ROS_WARN("Error Loading output File");
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
				  if(i==11)
				  {
					  	for(int j=0; j<12;j++)
					  	{
					  		if(j ==0 )//adjust x value
						  {
							  outFile<<data[j]-tool_len*data[9];
							  // outFile<<data[j]/1000.;
						  }
							  
						  else if (j==1)//adjust y value
						  {
							   outFile<<data[j]-tool_len*data[10];
							   // outFile<<data[j]/1000.;
						  }
							 
						  else if(j==2)//adjust z value
						  {
							   outFile<<data[j]-tool_len*data[11];
							   // outFile<<data[j]/1000.;
						  }
						  else if(j>2)//keep orientation values
							  outFile<<(data[j]);
						  
						  if(j!=11)
							  outFile<<",";
						  else if(j==11)
							  outFile<<"\n";
					   }
				  	}


					  
			  }
			  
			}
			outFile.close();

			ROS_INFO("output file closed");  
			ROS_INFO("output file generated");  
		}

	return 0;
}
// %Tag(fullSource)%
