#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"
#include "std_srvs/Empty.h"


class monitor
{

public:
geometry_msgs::Twist vel;
ros::NodeHandle nh;
ros::Publisher vel_pub;
void setup(ros::NodeHandle &nh);

ros::Subscriber laser_sub;
ros::Subscriber vel_sub;
void vel_callback(const geometry_msgs::Twist::ConstPtr & msg);
void laser_callback(const sensor_msgs::LaserScan::ConstPtr& msg);
void Is_Monitor(bool trigger);
bool Is_Monitor_on;

private:



void _Pass_Vel();
bool _Is_Warn;
void _Speed_Control();
void _Stop();
};
//启动函数，初始化订阅发布功能
void monitor::setup(ros::NodeHandle &nh){
	vel_pub=nh.advertise<geometry_msgs::Twist>("/velocity",5);
	vel_sub=nh.subscribe<geometry_msgs::Twist>("cmd_vel",1,&monitor::vel_callback,this);//注意，这里的回调函数没表类型，可能会报错。
	
	if(Is_Monitor_on){
	laser_sub=nh.subscribe<sensor_msgs::LaserScan>("scan",1,&monitor::laser_callback,this);
	ROS_INFO("Starting Monitor, succeed!!");}
	else
	ROS_INFO("Closing Monitor, completed!!");

	ROS_INFO("initial completed");
};

//回调函数 获取速度，不受monitor直接控制，但速度发布内容可以受到雷达信息控制！！！
void monitor::vel_callback(const geometry_msgs::Twist::ConstPtr& msg){
	//接受速度信号，传递进入类的速度成员变量
	this->vel.linear=msg->linear;
	this->vel.angular=msg->angular;
	if(Is_Monitor_on)
	//速度限制功能
	_Speed_Control();
	else
	//速度直接传递
	_Pass_Vel();
};


//回调函数 获取雷达信号，受monitor控制
void monitor::laser_callback(const sensor_msgs::LaserScan::ConstPtr &msg){
	//接受雷达型号，判断是否处于警戒距离。执行积极停止或者不执行。
    _Is_Warn=false;
	int i=0;
	while(i<360)//这里值得优化。i<实际扫描个数
    {
         if(msg->ranges[i]<0.4){
            i++;
            _Is_Warn=true;
            break;
      }
         else
         i++;
    };
};


//开关函数，将launch定义的参数值赋予成员变量，which is决定是否采用速度监控功能
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
 //执行停止操作
{
ROS_ERROR("Warning!! Must STOP!!");
    _Stop();
vel_pub.publish(vel);}
    else{
ROS_INFO("GO AHEAD~~");
    _Pass_Vel();}
vel_pub.publish(vel);;

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
/*std::string input_vel_topic="cmd_vel";
 
std::string output_vel_topic="velocity";
std::string input_laser_topic="scan";*/

	ros::init(argc,argv,"monitor");
	ros::NodeHandle nh;
        ros::Rate r=10;

nh.param<bool>("trigger", trigger, true);
/*nh_private.param<std::string>("input_vel_topic", input_vel_topic, "cmd_vel");
nh_private.param<std::string>("output_vel_topic", output_vel_topic, "velocity");
nh_private.param<std::string>("input_laser_topic", input_laser_topic, "scan");*/
	/*这里要有一堆ros参数定义
	*/
	monitor moni;
	moni.Is_Monitor(trigger);
	moni.setup(nh);
	



	while(nh.ok()){

	ros::spinOnce();
	//moni.vel_pub.publish(moni.vel);
	r.sleep();
//这里可以添加速度归0，什么破输入法！或者把发布放在速度回调函数中，实现有输入才有输出。我选择把上2行的发布函数，写入回调里面
	}
	return 0;
}
