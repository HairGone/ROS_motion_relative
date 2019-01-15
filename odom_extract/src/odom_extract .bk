#include <ros/ros.h>
#include <nav_msgs/Odometry.h>

class odom_new
{
public:
//odom_new(){};
nav_msgs::Odometry odom1;

ros::NodeHandle nh;
ros::Subscriber odom_sub;
ros::Subscriber odom_combine_sub;
ros::Publisher odom_pub; 

void setodom(const nav_msgs::Odometry::ConstPtr& msg){this->odom1.twist=msg->twist;ROS_INFO("got twist of odom");};
void setodom_combine(const geometry_msgs::PoseWithCovariance::ConstPtr& msg){this->odom1.pose.pose=msg->pose;this->odom1.pose.covariance=msg->covariance;ROS_INFO("got pose of odom");};

void setup(ros::NodeHandle &nh){odom_sub= nh.subscribe<nav_msgs::Odometry>("odom", 1, &odom_new::setodom,this);
odom_combine_sub= nh.subscribe<geometry_msgs::PoseWithCovariance>("odom_combine", 1, &odom_new::setodom_combine,this);


odom_pub      = nh.advertise<nav_msgs::Odometry>("odom_new", 5);
odom_pub.publish(odom1);
ROS_INFO("hi");};
};
//void odom_new::setodom(const nav_msgs::Odometry::ConstPtr& msg)
//{};



int main(int argc,char **argv)
{
ros::init(argc,argv,"odom_new");
ros::NodeHandle nh;
odom_new a;
a.setup(nh);
ROS_INFO("fck");
ros::spin();
return 0;
}
