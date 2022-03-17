#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Char.h>

void chatter1(const std_msgs::Float32::ConstPtr& msg)
{
  printf("I heard [%f]\n", msg->data);
 
}



int main(int argc, char **argv)
{
  ros::init(argc, argv, "std_msgs_listener");
  ros::NodeHandle n;
  ros::Subscriber sub15 = n.subscribe("/float32", 1000, chatter1);
  
  ros::spin();

  return 0;
}
