#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include "pigpio.h"
#include <ros/ros.h>
#include <ackermann_msgs/AckermannDrive.h>

/*four states per motor
low low stop
low high forward
high low backward
high high brake
*/

#define MOTOR_L_M    9
#define MOTOR_L_E    11
#define MOTOR_R_M    8
#define MOTOR_R_E    7

static int version, micros=5, millis=100;
static int gpio_in1[2]={MOTOR_L_M, MOTOR_R_M};
static int gpio_in2[2]={MOTOR_L_E, MOTOR_R_E};
static int speed[2]={140,140};
static float radius=2.0;//radius of the wheels
static float b=4.0;//distance between wheel and turning point

void motorTick(void)
{
    for (int i=0; i<2; i++)
    {
        gpioPWM(gpio_in1[i], 0);
        gpioPWM(gpio_in2[i], speed[i]);
    }
    ROS_INFO("Tick left: %d right: %d ",speed[0], speed[1]);
}

void servoboard_setservo(int servonr,int value){
    speed[servonr]=value;
}

void msgCallback(const ackermann_msgs::AckermannDrive::ConstPtr& msg)
{
    ROS_INFO("Speed %f at steering angle %f ",msg->speed, msg->steering_angle);

    float omega_base=msg->speed/radius;
    float omega_left;
    float omega_right;
    float deltaomega = msg->steering_angle*2*b/radius;

    if(msg->steering_angle>=0){
        omega_right=omega_base+(deltaomega/2);
        omega_left=omega_base-(deltaomega/2);
    } else {
        omega_right=omega_base-(deltaomega/2);
        omega_left=omega_base+(deltaomega/2);
    }

    servoboard_setservo(0,omega_left);
    servoboard_setservo(1,omega_right);

    ROS_INFO("Omega left: %f right: %f ",omega_left, omega_right);
}

int main(int argc, char *argv[])
{
   //init pigpio
   if (argc > 1) micros = atoi(argv[1]);
   if (argc > 2) millis = atoi(argv[2]);
   gpioCfgBufferSize(millis);
   gpioCfgClock(micros, PI_CLOCK_PCM, PI_CLOCK_PLLD);
   version = gpioInitialise();
   if (version >= 0)
    {
        /* initialise pins, only gpio numbers are supported */
        gpioSetMode(MOTOR_L_M,   PI_OUTPUT);
        gpioSetMode(MOTOR_L_E,   PI_OUTPUT);
        gpioSetMode(MOTOR_R_M,   PI_OUTPUT);
        gpioSetMode(MOTOR_R_E,   PI_OUTPUT);
        /* update motors every 100 msec, timer #2 */
        gpioSetTimerFunc(0, 1000, motorTick);
    }
    
   //init ROS
   ros::init(argc, argv, "legocar_driver_node");
   ros::NodeHandle n;
   ros::Subscriber sub = n.subscribe("cmd_vel", 1000, msgCallback);
    ros::spin();
    gpioTerminate();
    return 0;
}
