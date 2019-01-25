#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"
#include "std_srvs/Empty.h"


class monitor
{

public:
geometry_msgs::Twist vel;
ros::NodeHandle nh;
void setup(ros::NodeHandle &nh);

ros::Subscriber laser_sub;
ros::Subscriber vel_sub;
void vel_callback(const geometry_msgs::Twist::ConstPtr & msg);
void laser_callback(const sensor_msgs::LaserScan::ConstPtr& msg);
void Is_Monitor(bool trigger);
bool Is_Monitor_on;

private:

int _right;
int _left;
int _front;

void _Pass_Vel();
bool _Is_Warn;
void _Speed_Control();
void _Stop();
};
void monitor::setup(ros::NodeHandle &nh){
	vel_sub=nh.subscribe<geometry_msgs::Twist>("cmd_vel",1,&monitor::vel_callback,this);
	
	if(Is_Monitor_on){
	laser_sub=nh.subscribe<sensor_msgs::LaserScan>("scan",1,&monitor::laser_callback,this);
	ROS_INFO("Starting Monitor, succeed!!");}
	else
	ROS_INFO("Closing Monitor, completed!!");

	ROS_INFO("initial completed");
};

void monitor::vel_callback(const geometry_msgs::Twist::ConstPtr& msg){

	this->vel.linear=msg->linear;
	this->vel.angular=msg->angular;
	if(Is_Monitor_on)

	_Speed_Control();
	else

	_Pass_Vel();
};


void monitor::laser_callback(const sensor_msgs::LaserScan::ConstPtr &msg){
	
    _Is_Warn=false;
	int i=120;
_right=0;_front=0;_left=0;
	while(i<240)
    {
         if(msg->ranges[i]<0.4&&(msg->ranges[i]!=std::numeric_limits<float>::infinity())){
            i++;
if (119<i&&i<160){
ROS_INFO("120-160 degree of zone has obstacles");
}
else if(i>159&&i<200){
ROS_INFO("160-200 degree of zone has obstacles");
}
else {
ROS_INFO("200-240 degree of zone has obstacles");
};
//ROS_INFO("dangerous angle is %d, and range is %f",i,msg->ranges[i]); //this is monitoring line
            _Is_Warn=true;
            //break;
      }
         else
         i++;
    };
};


void monitor::Is_Monitor(bool trigger){
	//将参数注册到param server 返回参数的值
        Is_Monitor_on=trigger;
	if(trigger)
	ROS_INFO("try to start monitor");
	else
	ROS_INFO("try to close monitor, convert speed directly to specified topic.");
};

void monitor::_Speed_Control()
    {if (_Is_Warn)
{
ROS_ERROR("Warning!! Must STOP!!");
    _Stop();
}
    else{
ROS_INFO("GO AHEAD~~");
    _Pass_Vel();}

}

void monitor::_Stop(){
this->vel.linear.x=0;
this->vel.linear.y=0;
this->vel.linear.z=0;
this->vel.angular.x=0;
this->vel.angular.y=0;
this->vel.angular.z=0;
}

void monitor::_Pass_Vel(){
//transist velocity by doing nothing
}




int main(int argc, char ** argv)
{

bool trigger=true;

	ros::init(argc,argv,"monitor");
	ros::NodeHandle nh;
        ros::Rate r=10;

nh.param<bool>("trigger", trigger, true);
	monitor moni;
	moni.Is_Monitor(trigger);
	moni.setup(nh);
	



	while(nh.ok()){

	ros::spinOnce();
	r.sleep();
	}
	return 0;
}
