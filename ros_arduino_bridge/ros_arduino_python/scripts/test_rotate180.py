#!/usr/bin/env python

import rospy
#from geometry_msgs.msg import Twist,Point,Quaternion
#import tf
from geometry_msgs.msg import Twist
from math import pi


class OutAndBack():
  def __init__(self):
    rospy.init_node('out_and_back',anonymous=False)
    rospy.on_shutdown(self.shutdown)
    self.cmd_vel=rospy.Publisher('/cmd_vel',Twist,queue_size=10)
    rate=50
    r=rospy.Rate(rate)
    linear_speed=0.2
    goal_distance=1.0
    linear_duration=goal_distance/linear_speed
    angular_speed=0.3
    goal_angle=pi
    n=1
    angular_duration=goal_angle/angular_speed
    #rospy.loginfo(angular_duration)

    
    move_cmd=Twist()
    self.cmd_vel.publish(move_cmd)
    rospy.sleep(1)
      #rospy.loginfo(move_cmd.angular.z)
    move_cmd.angular.z=n*angular_speed
    ticks=int(angular_duration*rate)
      #n-=1
      #rospy.loginfo(move_cmd.angular.z)
    for t in range(ticks):
      self.cmd_vel.publish(move_cmd)
      r.sleep()
    move_cmd=Twist()
    self.cmd_vel.publish(move_cmd)
    rospy.sleep(1)
      #rospy.loginfo("one time")

  def shutdown(self):
      rospy.loginfo("stopping the robot")
      self.cmd_vel.publish(Twist())
      rospy.sleep(1)

if __name__=='__main__':
   try:
      OutAndBack()
   except:
      rospy.loginfo("out-and-back failed")
