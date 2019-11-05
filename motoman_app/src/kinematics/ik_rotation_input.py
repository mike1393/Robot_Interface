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


def ik_solver():
	pub = rospy.Publisher('coodinator_msgs_to_gui', String, queue_size=1000)
	rospy.init_node('ik_solver_gp8', anonymous=True)
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
	#


	# Read in file data
	# data format 
	# x,y,z a,b,c
	# x, y, z for postion and a, b, c for Rotation matrix 
	# the position value is stored in pos array 
	# the angle value is stored in rot array
	# return pos array and rot array 
	filename =rospack.get_path('object_viz')+"/src/data/tool_base_value.csv"
	print "filename"
	out_filename = rospack.get_path('motoman_app')+"/joint_data/joint_value_gp8_data.csv"
	break_out_file = rospack.get_path('motoman_app')+"/joint_data/break_point_data.csv"
	in_value=np.genfromtxt(filename, delimiter=',')# n by 12 array
	print "Data stored"
	(row,col) = in_value.shape
	print "Data length: ", row
	#

	#inverse kinematic calculation returns 88% of joint values
	print "\n ik calculating ... \n"
	q= np.array([0.0, 0.0, 0.0, 0.0, 0.0, 0.0])# initial angle
	error_count =0
	lost=1
	lost_count=0
	q_sol=np.array([])
	home_value=50/1000.#the clearance value for changing trajectory
	q_break=np.array([])
	max_attemp=100
	low_bound=np.array([-1.64,-0.90,-1.22,-1.68,-1.95, -6.25])#6.25
	high_bound=np.array([1.64, 1.12, 0.6, 1.68, 1.44, 6.25])
	loop=0;
	i=-1
	while i < row:
		print "current i: ", i
		if i==-1:
			print "start"
			pos=in_value[0,:3]/1000.
			rot = in_value[0,3:6]# Rx values
			rot = np.vstack([rot,in_value[0,6:9]])# Ry values
			rot = np.vstack([rot,in_value[0,9:]])# Rz values
			pos[0]=pos[0]-home_value*rot[2,0]
			pos[1]=pos[1]-home_value*rot[2,1]
			pos[2]=pos[2]-home_value*rot[2,2]
			q_ik = kdl_kin.inverse_input_R(pos, rot,q, min_joints=low_bound, max_joints=high_bound)
			if q_ik is None:
				q_ik = kdl_kin.inverse_input_search_R(pos, rot,timeout=1., min_joints=low_bound, max_joints=high_bound)
		else:
			pos=in_value[i,:3]/1000.
			rot = in_value[i,3:6]# Rx values
			rot = np.vstack([rot,in_value[i,6:9]])# Ry values
			rot = np.vstack([rot,in_value[i,9:]])# Rz values
			q_ik = kdl_kin.inverse_input_R(pos, rot,q,min_joints=low_bound, max_joints=high_bound)# inverse kinematics with previous angle 
			# if the solution cannot be found
			# which might happen at corners 
			if q_ik is None:
				print "continuity break at #", i
				pos[0]=pos[0]-home_value*rot[2,0]
				pos[1]=pos[1]-home_value*rot[2,1]
				pos[2]=pos[2]-home_value*rot[2,2]
				q_ik = kdl_kin.inverse_input_search_R(pos, rot,timeout=1., min_joints=low_bound, max_joints=high_bound)
				if q_ik is not None:

					if len(q_break)==0:
						q_break=np.append(q_break, [i])
					else:
						if q_break[-1]!=i:
							q_break=np.append(q_break, [i])
						else:
							q_sol=np.delete(q_sol, -1, axis=0)
					print "q_break:", q_break	
					i=i-1
					loop=loop+1
				if q_ik is None:
					print "fail"
					error_count=row
					break
				if loop >5:
					break
					
		if q_ik is not None:
			if len(q_sol)==0:
				q_sol = q_ik
				print "q_sol: ", q_sol
			else:
				q_sol = np.vstack([q_sol,q_ik])
				q = q_ik
				#make q_ik the new initial angle

		else:
				error_count+=1
				print "calculation break at #", i
		i=i+1


		sys.stdout.write("\r" + "[Progress]: " + str(i/(1.*row)*100.)+"%")
		sys.stdout.flush()





	print "q_sol generated"
	print "error_count: " , error_count 
	print "ik_accuracy: ", 1.0-(error_count/(1.*row))
	ik_accuracy=1.0-(error_count/(1.*row))


	#
	if ik_accuracy>0.7:
		(length,width) = q_sol.shape
		# save ik result to txt file
		# write q_ik to new data
		np.savetxt(out_filename,q_sol,fmt='%f',delimiter=",", header=str(length))
		np.savetxt(break_out_file,q_break,fmt='%d',delimiter=",", header=str(len(q_break)))
		print "joint_data stored"
		print "break_num: ", len(q_break)
		result_str="Calculation Succeed\nJoint Data Stored"
		pub.publish(result_str)
		
	else:
		result_str="Path Failed\nPlease change part position"
		pub.publish(result_str)
		print "path fail"

	print "done."
	



if __name__ == '__main__':
	try:
		ik_solver()
	except rospy.ROSInterruptException:
		pass



