 #include "ros/ros.h"
 #include "std_msgs/Int8.h"
 #include "std_msgs/Bool.h"    //boolean data
 #include "std_msgs/String.h"
 #include "sensor_msgs/Range.h"  //ultrasonic sensor message
 #include "geometry_msgs/Twist.h"
 
 std_msgs::Bool flag_AEB;
 
 
  void UltraSonarCallback2(const std_msgs::Bool::ConstPtr& msg)
 {
	 ROS_INFO("Sonar Range: [%f]", msg->data);

	 if(msg->data)
	 {
		 ROS_INFO("AEB_Activation!!");
	 }
	 else
	 {
		 ROS_INFO("AEB_Activation_Off");
	 }
 }
 
 int main(int argc, char **argv)
 {
	 int count = 0;
	 
	 ros::init(argc, argv, "aeb_controller");
	 
	 ros::NodeHandle n;
	 
	 ros::Rate loop_rate(1); //10
	  
	 ros::Subscriber sub1 = n.subscribe("/RangeSonar1", 1000, UltraSonarCallback2);
	 
	 while (ros::ok())
	 {
		 loop_rate.sleep();
		 ros::spinOnce();
		 ++count;
	 }
	 return 0;
 }
