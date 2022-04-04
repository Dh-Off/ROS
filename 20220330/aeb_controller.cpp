#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Bool.h"  	//bool
#include "std_msgs/String.h"
#include "sensor_msgs/Range.h"   //ultrasonic sensor message
#include "geometry_msgs/Twist.h"  //cmd_vel
#include "nav_msgs/Odometry.h"

#define frequency_odom_pub 50  //hz
#define stop 5

std_msgs::Bool flag_AEB;
geometry_msgs::Twist cmd_vel_msg;
std_msgs::Float32 delta_range;
std_msgs::Float32 old_sonar_range;
nav_msgs::Odometry pos, delta_pos, past_pos, sp;
nav_msgs::Odometry estimated_odom;


float x,y;
float delta_x, delta_y;				 // vehicle position[m/s]
float vx,vy; 						 // vehicle velocity[m/s]
float aeb_collision_distance = 200;  // aeb_engagementdistance[m]

void odomCallback(const nav_msgs::Odometry& msg)
{	
	
	estimated_odom.twist.twist.linear.x = msg.twist.twist.linear.x;
	estimated_odom.twist.twist.linear.y = msg.twist.twist.linear.y;
	estimated_odom.twist.twist.linear.z = msg.twist.twist.linear.z;
	
	estimated_odom.twist.twist.angular.x = msg.twist.twist.angular.x;
	estimated_odom.twist.twist.angular.y = msg.twist.twist.angular.y;
	estimated_odom.twist.twist.angular.z = msg.twist.twist.angular.z;
	
	float old_x, old_y;
	old_x = x;
	old_y = y; 
	
	
	ROS_INFO("Pos : x-[%.2lf] y-[%.2lf]", msg.pose.pose.position.x, msg.pose.pose.position.y);
	
	x = msg.pose.pose.position.x;
	y = msg.pose.pose.position.y;
	
	delta_x = x - old_x;
	delta_y = y - old_y;
	
	vx = delta_x * frequency_odom_pub;
	vy = delta_y * frequency_odom_pub;
	
	if(stop <= x) {
		cmd_vel_msg.linear.x = 0;
		ROS_INFO("Stopped");
	}
	
	//ROS_INFO("Pos : x-[%.2lf] y-[%.2lf]", msg.pose.pose.position.x, msg.pose.pose.position.y);
	pos.pose.pose.position.x = msg.pose.pose.position.x;
	pos.pose.pose.position.y = msg.pose.pose.position.y;
	
	delta_pos.pose.pose.position.x = pos.pose.pose.position.x - past_pos.pose.pose.position.x ;
	delta_pos.pose.pose.position.y = pos.pose.pose.position.y - past_pos.pose.pose.position.y ;
	
	//ROS_INFO("Delta Pos : x-[%.2lf] y-[%.2lf]", delta_pos.pose.pose.position.x,delta_pos.pose.pose.position.y);
	
	past_pos.pose.pose.position.x  = pos.pose.pose.position.x;
	past_pos.pose.pose.position.y  = pos.pose.pose.position.y;
	
	sp.pose.pose.position.x = delta_pos.pose.pose.position.x / 0.02; //vx=x/hz  5.0=>0.02
	sp.pose.pose.position.y= delta_pos.pose.pose.position.y / 0.02; 
	
	//ROS_INFO("Vx: [%.2f] Vy: [%.2f]", sp.pose.pose.position.x, sp.pose.pose.position.y);
	
}

void UltraSonarCallback(const sensor_msgs::Range::ConstPtr& msg)
{
	//ROS_INFO("Sonar Seq: [%d]", msg->header.seq);
	//ROS_INFO("Sonar Range: [%f]", msg->range);
	
	aeb_collision_distance = vx *(0.7 + 0.1) * 0.22778 * 2.5;   // 1msec = 0.22778 km/h  safe-last  **2.1-5
	if(msg->range<= aeb_collision_distance)
	{
		//ROS_INFO("AEB_Activaton!!");
		flag_AEB.data=true;
	}
	
	else
	{
			flag_AEB.data = false;
	}
	delta_range.data = msg->range-old_sonar_range.data;
    //ROS_INFO("delta_range : [%f]", delta_range.data);
    old_sonar_range.data = msg->range;
}
	
	void CarControlCallback(const geometry_msgs::Twist& msg)
{
	//ROS_INFO("Cmd_vel : linear x [%f]", msg.linear.x);

	cmd_vel_msg = msg;
	//ROS_INFO("Cmd_vel : linear x [%f]", cmd_vel_msg.linear.x);
}
	
	void UltraSonarCallback2(const sensor_msgs::Range::ConstPtr& msg)
{
	//ROS_INFO("Sonar Seq: [%d]", msg->header.seq);
	//ROS_INFO("Sonar Range: [%f]", msg->range);
}


int main(int argc, char **argv)
{
	pos.pose.pose.position.x = 0.0;
	pos.pose.pose.position.y = 0.0;
	
	int count = 0;
	
	ros::init(argc, argv, "aeb_controller");
	
	ros::NodeHandle n;
	
	std::string odom_sub_topic = "/ackermann_steering_controller/odom";
	
	
	ros::Subscriber sub = n.subscribe("/range",1000,UltraSonarCallback);
	ros::Subscriber sonar_sub = n.subscribe("/Rangesonar1",1000,UltraSonarCallback2);
	ros::Subscriber cmd_vel_sub = n.subscribe("/cmd_vel",10,&CarControlCallback);
	
	ros::Subscriber sub_odom = n.subscribe(odom_sub_topic, 10, &odomCallback);
	
	
	
	ros::Publisher pub__vel_speed = n.advertise<geometry_msgs::Twist>("/vel_speed",10);
	ros::Publisher pub_delta_range = n.advertise<std_msgs::Float32>("/delta_range", 1000);
	ros::Publisher pub_cmd_vel = n.advertise<geometry_msgs::Twist>("/ackermann_steering_controller/cmd_vel",10);
	ros::Publisher pub_aeb_activation_flag=n.advertise<std_msgs::Bool>("/aeb_activation_flag",1);
	ros::Publisher pub_estimated_odom = n.advertise<nav_msgs::Odometry>("/estimated_odom", 10);
	

	ros::Rate loop_rate(2); //10
	
	while(ros::ok())
	{
		pub_estimated_odom.publish(estimated_odom);
		
		if((count%10)==0)
		{
			pub_aeb_activation_flag.publish(flag_AEB);
		}
		
		
		if(flag_AEB.data ==true)
		{
			if(cmd_vel_msg.linear.x>0) cmd_vel_msg.linear.x = 0;
			pub_cmd_vel.publish(cmd_vel_msg);
		}
		else
		{
			pub_cmd_vel.publish(cmd_vel_msg);
		} 
		
		//collision_distance = vx * (1/10);
		
		ROS_INFO("Odom : [%6.3f %6.3f] m | Velocity : [%6.3f %6.3f] m/s", x,y,vx,vy);
		ROS_INFO("Collision_Distance : %6.3f",aeb_collision_distance);
		
		loop_rate.sleep();
		ros::spinOnce();
		++count;
	}
	
	return 0;
}
