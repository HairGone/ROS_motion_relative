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

// callback for transition odom-twist to odom_new-twist
void setodom(const nav_msgs::Odometry::ConstPtr& msg){this->odom1.twist=msg->twist;ROS_INFO("got twist of odom");};

// callback for transition odom_combine-pose to odom_new-pose
void setodom_combine(const geometry_msgs::PoseWithCovariance::ConstPtr& msg){this->odom1.pose.pose=msg->pose;this->odom1.pose.covariance=msg->covariance;ROS_INFO("got pose of odom");};

// initial function to set up a publisher and two subscriber
//note: queue size need to be rightly set.  
void setup(ros::NodeHandle &nh)
{
odom_sub= nh.subscribe<nav_msgs::Odometry>("odom", 1, &odom_new::setodom,this);
odom_combine_sub= nh.subscribe<geometry_msgs::PoseWithCovariance>("odom_combine", 1, &odom_new::setodom_combine,this);
odom_pub      = nh.advertise<nav_msgs::Odometry>("odom_new", 5);
ROS_INFO("hi");};
};



int main(int argc,char **argv)
{
// create a node named odom_new
ros::init(argc,argv,"odom_new");
ros::NodeHandle nh;
// build object in order to publish combined odom information
odom_new a;
// init
a.setup(nh);
ROS_INFO("fck_begining");
//publishing rate
ros::Rate r(1.0);


while(nh.ok()){
//msgs subscribed by subscribers will not be utilized by callback function untill ros::spin/spinOnce active them 
ros::spinOnce();

//publish nav_msgs::Odometry a.odom1
a.odom_pub.publish(a.odom1);

//loop
r.sleep();
}
return 0;
}

/* there is a serious problem of time synchronizing that has not been solved.
using message::filter instead of ros::subscriber is one hint.
also, resources of timesychronizer will be helpful
*/
