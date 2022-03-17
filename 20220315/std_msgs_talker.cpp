#include <ros/ros.h>
#include <ros/xmlrpc_manager.h>
#include <std_msgs/UInt32.h>
#include <sstream>


int main(int argc, char **argv)
{
  ros::init(argc, argv, "std_msgs_talker");
  ros::NodeHandle n;
  ros::Publisher chatter1 = n.advertise<std_msgs::UInt32>("uint32", 1000);

  

  ros::Rate loop_rate(1);
  std::cout<<ros::XMLRPCManager::instance()->getServerURI()<<std::endl;


  while (ros::ok())
  {
	int count = 0;
	int c = 0;
    std_msgs::UInt32 msg1;
    std_msgs::UInt32 msg2;
    
    std::cin>>count;
    std::cin>>c;
    
    msg1.data = count+c;
    chatter1.publish(msg1);
    
    msg1.data = count-c;
    chatter1.publish(msg1);
    
    msg1.data = count*c;
    chatter1.publish(msg1);
    
    msg1.data = count/c;
    chatter1.publish(msg1);
    
  }
  

  return 0;
}
