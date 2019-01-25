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
void Is_Monitor(bool trigger,double range,double acc);
bool Is_Monitor_on;

private:
double _acc;
int _right;
int _left;
int _front;
int _combine;
double _range;
void _Pass_Vel();
bool _Is_Warn;
void _Speed_Control();
void _Stop();
void _vel_control();

void _turn_left();
void _turn_left_hard();
void _turn_right();
void _turn_right_hard();

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
	int i=120;
_right=0;_front=0;_left=0;
	while(i<240)//这里值得优化。i<实际扫描个数
    {
         if(msg->ranges[i]<_range&&(msg->ranges[i]!=std::numeric_limits<float>::infinity())){
            i++;
                if (119<i&&i<160){
                _right=1;
                  //ROS_INFO("120-160 degree of zone has obstacles");
                                 }
                 else if(i>159&&i<200){
                _front=1;
                  //ROS_INFO("160-200 degree of zone has obstacles");
                  }
                 else {
                 _left=1;
                 //ROS_INFO("200-240 degree of zone has obstacles");
                 };
//ROS_INFO("dangerous angle is %d, and range is %f",i,msg->ranges[i]); //this is monitoring line
            _Is_Warn=true;
            //break;
      }
         else
         i++;
    };
_combine=100*_right+10*_front+_left;
//ROS_INFO("obstacle zones is  %d",_combine);

//_vel_control();

};


//开关函数，将launch定义的参数值赋予成员变量，which is决定是否采用速度监控功能
void monitor::Is_Monitor(bool trigger,double range,double acc){
	//将参数注册到param server 返回参数的值
        Is_Monitor_on=trigger;
	if(trigger)
	ROS_INFO("try to start monitor");
	else
	ROS_INFO("try to close monitor, convert speed directly to specified topic.");
       
_range=range;
_acc=acc;
ROS_INFO("range acc is %f %f", range, acc);
};
//if else标点不要忘；大小写不要错；

void monitor::_Speed_Control()
    {if (_Is_Warn)
 //执行停止操作
{
ROS_ERROR("Warning!! avoidance is excuting!");
    //_Stop();
_vel_control();
vel_pub.publish(vel);
}
    else{
ROS_INFO("GO AHEAD~~");
    _Pass_Vel();}
vel_pub.publish(vel);;

}

void monitor::_vel_control(){

switch (_combine){
case 0:
ROS_INFO("GO AHEAD~~");
 _Pass_Vel();// revise needed
break;
case 100:
ROS_INFO("right obstacle, slightly turn left");
_turn_left();
break;
case 10:
ROS_INFO("front obstacle, slightly turn around");
_Stop();// revise needed
break;
case 1:
ROS_INFO("left obstacle,slightly turn right");
_turn_right();
break;
case 11:
ROS_INFO("front left obstacle, hardly turn right ");
_turn_right_hard();
break;
case 110:
ROS_INFO("front right obstacle, hardly turn left");
_turn_left_hard();
break;
case 111:
ROS_INFO("stop!!!");
_Stop();// revise needed
break;
case 101:
ROS_INFO("slightly go ahead");
 _Pass_Vel();// revise needed
break;

}
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
}



/**************避障速度控制******************/

/* 偏转函数 */
void monitor::_turn_left(){
	//int time=0;
this->vel.angular.z+=_acc;////这里的速度应该是递增，但是因为我引用的是订阅的速度信息，不是前一时刻的速度信息。所以目前速度无法递增。处理方法是设置时间戳
//if (this->vel.angular.z>2)
//this->vel.angular.z=1.5;
}

void monitor::_turn_right(){
	
this->vel.angular.z-=_acc;
ROS_INFO("z is %f",this->vel.angular.z);
//if (this->vel.angular.z<-1.5)
//this->vel.angular.z=-1.5;
}

void monitor::_turn_right_hard(){
this->vel.angular.z-=(_acc+2);
//if (this->vel.angular.z<-1.5)
//this->vel.angular.z=-2;
}

void monitor::_turn_left_hard(){
	
this->vel.angular.z+=(_acc+2);
//if (this->vel.angular.z>1.5)
//this->vel.angular.z=2;
}
/*偏转函数*/



int main(int argc, char ** argv)
{
double range=0.4;
double acc=2.0;
bool trigger=true;// the reason why param is not load is namespace!
	ros::init(argc,argv,"fcking_bonus");
	ros::NodeHandle nh;
        ros::Rate r=10;
ros::NodeHandle nh_private("~");
nh_private.param<bool>("trigger", trigger, true);
nh_private.param<double>("range", range,22);
ROS_INFO("range is xxx%f",range);
nh_private.param<double>("acc",acc,22);//param serves to transist param oflaunch file into main loop and register it in param servers. Simply said, param cmd is to give variable a new value which u define in launch file.
ROS_INFO("acc is xxx%f",acc);
	monitor moni;
	moni.Is_Monitor(trigger,range,acc);
	moni.setup(nh);
	while(nh.ok()){
	ros::spinOnce();
	r.sleep();
	}
	return 0;
}
