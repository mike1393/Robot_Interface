#!/usr/bin/env python

#  Author: Yi-Wei, Chen
#  Email: yiweiche@usc.edu
#  ------------------------------------
import rospy
from sensor_msgs.msg import JointState

from urdf_parser_py.urdf import URDF
from pykdl_utils.kdl_kinematics import KDLKinematics
from hrl_geom.pose_converter import PoseConv
from std_msgs.msg import String
import PyKDL as kdl
import numpy as np
import random
import sys
import rospkg

#@param q previous joint trajectory
#@param q_ik current joint trajectory
#@param pos targeted position
#@param rot targeted frame orientation
#@param tol tolerance value between previous and current value of angles (unit: degree)




def link_trans():
	pub = rospy.Publisher('coodinator_msgs_to_gui', String, queue_size=1000)
	rospy.init_node('link_trans_gp8', anonymous=True)
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

	link_files=[]
	for i in range(8):
		link_files.append(rospack.get_path('motoman_app')+"/joint_data/link_"+str(i)+"_data.csv")
		print link_files[i]
	joint_filename =rospack.get_path('motoman_app')+"/joint_data/joint_value_gp8_data.csv"
	print "joint_filename"
	home_pose_file=rospack.get_path('motoman_app')+"/joint_data/home_pose.csv"
	in_value=np.genfromtxt(joint_filename, delimiter=',')# n by 12 array
	print "Data stored"
	(row,col) = in_value.shape
	print "Data length: ", row
	#

	q_init=np.array([0.0,0.0,0.0,0.0,0.0,0.0])
	tf2=np.array([])
	for i in range(8):
		pose=kdl_kin.forward(q_init,kdl_kin.get_link_names()[i],kdl_kin.get_link_names()[0])
		pos, rot = PoseConv.to_pos_rot(pose)
		tf_temp=np.array([pos[0,0],pos[1,0],pos[2,0],rot[0,0],rot[1,0],rot[2,0],rot[0,1],rot[1,1],rot[2,1],rot[0,2],rot[1,2],rot[2,2]])
		if len(tf2)==0:
			tf2=tf_temp
		else:
			tf2=np.vstack([tf2,tf_temp])
	print "home_pose done"
	np.savetxt(home_pose_file,tf2,fmt='%f',delimiter=",")

	for i in range(8):
		tf=np.array([])
		for j in range(row):
			q=in_value[j]
			# pose=kdl_kin._do_kdl_fk(q, i)
			pose=kdl_kin.forward(q,kdl_kin.get_link_names()[i],kdl_kin.get_link_names()[0])
			pos, rot = PoseConv.to_pos_rot(pose)
			tf_temp=np.array([pos[0,0],pos[1,0],pos[2,0],rot[0,0],rot[1,0],rot[2,0],rot[0,1],rot[1,1],rot[2,1],rot[0,2],rot[1,2],rot[2,2]])
			if j==0:
				tf=tf_temp

			else:
				tf=np.vstack([tf,tf_temp])
		print "link", i, " done"
		(length,width) = tf.shape
		np.savetxt(link_files[i],tf,fmt='%f',delimiter=",")
		print "link", i, "saved"

	result_str="link_saved"
	pub.publish(result_str)
	



if __name__ == '__main__':
	try:
		link_trans()
	except rospy.ROSInterruptException:
		pass



