#include <ros/ros.h>
#include<geometry_msgs/PoseWithCovariance.h>
 int main(int argc, char** argv)
{
      ros::init(argc, argv, "odometry_publisher2");
      ros::NodeHandle n;
      ros::Publisher odom_combine_pub = n.advertise<geometry_msgs::PoseWithCovariance>("odom_combine", 50);
//      tf::TransformBroadcaster odom_broadcaster;
      double x = 0.0;
      double y = 0.0;
      double th = 0.0;
      double vx = 0.01;
      double vy = -0.01;
      double vth = 0.01;
  //      current_time = ros::Time::now();
    //  last_time = ros::Time::now();
      ros::Rate r(1.0);
      while(n.ok())
{
        ros::spinOnce();               // check for incoming messages
      //  current_time = ros::Time::now();
        //compute odometry in a typical way given the velocities of the robot
       // double dt = (current_time - last_time).toSec();
       // double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
       // double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
       // double delta_th = vth * dt;
     // x += delta_x;
       // y += delta_y;
       // th += delta_th;
     
        //since all odometry is 6DOF we'll need a quaternion created from yaw
//        geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);
        //first, we'll publish the transform over tf
       // geometry_msgs::TransformStamped odom_trans;
       // odom_trans.header.stamp = current_time;
       // odom_trans.header.frame_id = "base_link";
       // odom_trans.child_frame_id = "odom_combine";
       // odom_trans.transform.translation.x = x;
       // odom_trans.transform.translation.y = y;
      //  odom_trans.transform.translation.z = 0.0;
    //    odom_trans.transform.rotation = odom_quat;
        //send the transform
  //      odom_broadcaster.sendTransform(odom_trans);
     
        //next, we'll publish the odometry message over ROS
        geometry_msgs::PoseWithCovariance odom;
       // odom.header.stamp = current_time;
       // odom.header.frame_id = "odom_combine";
     
        //set the position
        odom.pose.position.x = x;
        odom.pose.position.y = y;
        odom.pose.position.z = 0.0;
        odom.pose.orientation.x = 0;
        odom.pose.orientation.y = 0;
        odom.pose.orientation.z = 0;
        odom.pose.orientation.w = 1;
     
        //set the velocity
       // odom.child_frame_id = "base_link";
       // odom.twist.twist.linear.x = vx;
       // odom.twist.twist.linear.y = vy;
       // odom.twist.twist.angular.z = vth;
     
        //publish the message
        odom_combine_pub.publish(odom);
     
//        last_time = current_time;
        r.sleep();
      }
    }
