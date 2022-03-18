 #include "ros/ros.h"
 #include "std_msgs/Int8.h"
 #include "std_msgs/Bool.h"    //boolean data
 #include "std_msgs/String.h"
 #include "sensor_msgs/Range.h"  //ultrasonic sensor message
 #include "geometry_msgs/Twist.h"
 
 std_msgs::Bool flag_AEB;
 
 void UltraSonarCallback(const sensor_msgs::Range::ConstPtr& msg)
 {
	 if(msg->range <=1.0)
	 {
		 flag_AEB.data = true;
	 }
	 else
	 {
		 flag_AEB.data = false;
	 }
 }

 
 int main(int argc, char **argv)
 { 
	 int count = 0;
	 
	 ros::init(argc, argv, "aeb_pub");
	 
	 ros::NodeHandle n;
	 
	 ros::Rate loop_rate(1); //10
	
	 ros::Publisher pub1= n.advertise<std_msgs::Bool>("bool",1000);
	 
	 ros::Subscriber sub = n.subscribe("range", 1000, UltraSonarCallback);
	 

	 
	 while (ros::ok())
	 {
		 pub1.publish(flag_AEB);
		 loop_rate.sleep();
		 ros::spinOnce();
		 ++count;
	 }
	 return 0;
 }
