#!/usr/bin/env python

#  Author: Yi-Wei, Chen
#  Email: yiweiche@usc.edu
#  ------------------------------------
import rospy
import trimesh
from std_msgs.msg import String
import PyKDL as kdl
import numpy as np
import sys
import rospkg
from pykdl_utils.kdl_kinematics import KDLKinematics
from urdf_parser_py.urdf import URDF
from std_msgs.msg import String
import time


def link_trans():
	rospy.init_node('link_viz', anonymous=True)
	rate = rospy.Rate(100)
	rospy.sleep(1)
	rospack=rospkg.RosPack()
	rospack.list() 
	# Read in robot description from parameter srv 
	# and build kinematic model
	robot = URDF.from_parameter_server()
	base_link = robot.get_root()
	end_link = robot.link_map.keys()[len(robot.link_map)-1]
	print "Root link: %s; End link: %s" % (base_link, end_link)
	kdl_kin = KDLKinematics(robot, base_link, end_link)
	print "Robot ik model built...\n"
	print len(kdl_kin.get_link_names())
	#
	mesh=[]
	path=rospack.get_path('motoman_gp8_support')+"/meshes/visual/"
	mesh.append(trimesh.load(path+"gp8_base_link.stl"))
	mesh.append(trimesh.load(path+"gp8_link_1_s.stl"))
	mesh.append(trimesh.load(path+"gp8_link_2_l.stl"))
	mesh.append(trimesh.load(path+"gp8_link_3_u.stl"))
	mesh.append(trimesh.load(path+"gp8_link_4_r.stl"))
	mesh.append(trimesh.load(path+"gp8_link_5_b.stl"))
	mesh.append(trimesh.load(path+"gp8_link_6_t.stl"))
	mesh.append(trimesh.load(rospack.get_path('motoman_app')+"/mesh/gp8_probe.STL"))
	mesh[7].apply_scale(0.001)

	link_files=[]
	in_value=[]
	for i in range(8):
		link_files.append(rospack.get_path('motoman_app')+"/joint_data/link_"+str(i)+"_data.csv")
		in_value.append(np.genfromtxt(link_files[i], delimiter=','))# n by 12 array
	
	(row,col) = in_value[0].shape
	print row

	#

	for i in range(row):
		for j in range(8):
			if i==row-1:
				tf=np.array([
							[in_value[j][i,3],in_value[j][i,6],in_value[j][i,9],in_value[j][i,0]],
							[in_value[j][i,4],in_value[j][i,7],in_value[j][i,10],in_value[j][i,1]],
							[in_value[j][i,5],in_value[j][i,8],in_value[j][i,11],in_value[j][i,2]],
							[0.0,0.0,0.0,1.0]
							])
			else:
				tf=np.array([
				[1.0,0.0,0.0,0],
				[0.0,1.0,0.0,0],
				[0.0,0.0,1.0,0],
				[0.0,0.0,0.0,1.0]
				])
			mesh[j].apply_transform(tf)
		if i== row-1:
			(mesh[0]+mesh[1]+mesh[2]+mesh[3]+mesh[4]+mesh[5]+mesh[6]+mesh[7]).show()
			time.sleep(0.1)

	value_home=[]
	home_file=rospack.get_path('motoman_app')+"/joint_data/home_pose.csv"
	for i in range(8):
		value_home.append(np.genfromtxt(home_file, delimiter=','))




if __name__ == '__main__':
	try:
		link_trans()
	except rospy.ROSInterruptException:
		pass



