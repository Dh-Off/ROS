#include <ros/ros.h>
#include <std_msgs/UInt32.h>
#include <std_msgs/Char.h>

void chatter1(const std_msgs::UInt32::ConstPtr& msg)
{
  printf("I heard [%i]\n", msg->data);
 
}



int main(int argc, char **argv)
{
  ros::init(argc, argv, "std_msgs_listener");
  ros::NodeHandle n;
  ros::Subscriber sub1 = n.subscribe("/uint32", 1000, chatter1);
  
  ros::spin();

  return 0;
}
