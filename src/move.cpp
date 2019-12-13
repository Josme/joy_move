#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include <iostream>

using namespace std;

class TeleopJoy{
	public:
		TeleopJoy();
	private:
		void callBack(const sensor_msgs::Joy::ConstPtr& joy);
		ros::NodeHandle n; //实例化节点
		ros::Publisher pub;
		ros::Publisher pub_car_vel;
		ros::Subscriber sub;
		double v_linear,v_angular;//控制乌龟(海龟？)的速度
		int axis_ang,axis_lin; //axes[]的按键

};

TeleopJoy::TeleopJoy()
{
	//  在TeleopJoy类的构造函数中，初始化了六个变量。

	//这两个变量由游戏杆 轴向输入
	n.param<int>("axis_linear",axis_lin,3);//默认axes[1]接受角速度
	n.param<int>("axis_angular",axis_ang,0);//默认axes[2]接收角度
	n.param<double>("vel_linear",v_linear,1);//默认线速度1 m/s
	n.param<double>("vel_angular",v_angular,1);//默认角速度1 单位rad/s

	//这两个变量是发布者和订阅者，发布者使用turtlesim::Velocity类型发布主题
	//订阅者通过名为joy的主题得到数据。处理游戏杆的节点会发布数据到这个主题
	pub = n.advertise<geometry_msgs::Twist>("turtle1/cmd_vel",1);//发送速度给乌龟
	pub_car_vel = n.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/teleop",1);
	sub = n.subscribe<sensor_msgs::Joy>("joy",10,&TeleopJoy::callBack,this);	
}

//每一次节点都会接受到一个消息，调用callBack函数
void TeleopJoy::callBack(const sensor_msgs::Joy::ConstPtr& Joy)
{
	//用vel为名称创建一个新的用于发布数据的变量
	geometry_msgs::Twist vel;

	//游戏杆轴向输入的变量被赋给vel
	vel.angular.z = Joy->axes[axis_ang]*v_angular;
	vel.linear.x = Joy->axes[axis_lin]*v_linear;

	ROS_INFO("linear:%.3lf angular:%.3lf",vel.linear.x,vel.angular.z);

	//调用pub.Publish(vel)发布主题
	pub.publish(vel);
	pub_car_vel.publish(vel);
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "TeleopJoy");

	TeleopJoy teleop_turtle;//创建一个TeleopJoy类实例

	ros::spin();
}
