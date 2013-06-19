#include <ros/ros.h>
#include <turtlesim/Velocity.h>
#include <sensor_msgs/Joy.h>
#include <ackermann_msgs/AckermannDrive.h>


class TeleopLegoCar
{
public:
  TeleopLegoCar();

private:
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
  
  ros::NodeHandle nh_;

  int linear_, angular_;
  double l_scale_, a_scale_;
  ros::Publisher turtle_pub_;
  ros::Publisher ackermann_pub_;
  ros::Subscriber joy_sub_;
  
};


TeleopLegoCar::TeleopLegoCar():
  linear_(1),
  angular_(2)
{

  nh_.param("axis_linear", linear_, linear_);
  nh_.param("axis_angular", angular_, angular_);
  nh_.param("scale_angular", a_scale_, a_scale_);
  nh_.param("scale_linear", l_scale_, l_scale_);

  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 100, &TeleopLegoCar::joyCallback, this);

  turtle_pub_ = nh_.advertise<turtlesim::Velocity>("turtle1/command_velocity", 1);
  ackermann_pub_ = nh_.advertise<ackermann_msgs::AckermannDrive>("cmd_vel", 1);
}

void TeleopLegoCar::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{

  //publish to cmd channel
  ackermann_msgs::AckermannDrive drv_msg;
  drv_msg.steering_angle = a_scale_*joy->axes[angular_];
  drv_msg.speed = l_scale_*joy->axes[linear_];
  ackermann_pub_.publish(drv_msg);
    
  //publish to simulator
  turtlesim::Velocity vel;
  vel.angular = a_scale_*joy->axes[angular_];
  vel.linear = l_scale_*joy->axes[linear_];
  turtle_pub_.publish(vel);
}


int main(int argc, char** argv)
{
  ros::init(argc, argv, "legocar_wiimote");
  TeleopLegoCar teleop_legocar;
  ros::spin();
}

/* TODO
 while True:
 buttons = wm.state['buttons']
 if (buttons & cwiid.BTN_B):
 #boost mode
 speedModifier=200
 speedModifier2=50
 else:
 speedModifier=150
 speedModifier2=100
 if (buttons & cwiid.BTN_2):
 #print((wm.state['acc'][1]-125))
 servos.setSpeeds((speedModifier - wm.state['acc'][1]),wm.state['acc'][1] -speedModifier2)
 elif (buttons & cwiid.BTN_1):
 #print ~(wm.state['acc'][1]-125)
 servos.setSpeeds(~(speedModifier - wm.state['acc'][1]),~(wm.state['acc'][1] -speedModifier2))
 else:
 #print("stop")
 servos.setSpeeds(0,0)
 time.sleep(0.2)
 */