#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Bool.h>


bool flag;


class Override
{
public:
	Override();
	
private:
	
	void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
	ros::NodeHandle nh_;
	ros::Publisher override_pub;
  ros::Subscriber joy_sub_;
	bool currentReading;
	bool lastReading;
};

Override::Override()
{

  override_pub = nh_.advertise<std_msgs::Bool>("override_status", 1);
  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &Override::joyCallback, this);
  
}



void Override::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
  
  std_msgs::Bool override_status;
  
  if (joy->buttons[4] == 0.0){
    currentReading = false;

  }else currentReading = true;

  if (currentReading && !lastReading) {
    flag=!flag;
    if (flag) {
      override_status.data = true;
    }
    else {
      override_status.data = false;
    }
  }
  lastReading = currentReading;
  override_pub.publish(override_status);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "override_node");
  Override override;
  ROS_INFO("Running");
  ros::spin();
}