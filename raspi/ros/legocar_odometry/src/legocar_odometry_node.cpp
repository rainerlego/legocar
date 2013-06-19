#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include "pigpio.h"
#include "ros/ros.h"
#include <tf/transform_broadcaster.h>
#include "nav_msgs/Odometry.h"


#define frontEncoder 8 //front encoder GPIO pin 
#define rearEncoder 25 //rear encoder GPIO pin 


static int version, micros=5, millis=100;
uint32_t frontTickCurrent, frontTickLast, rearTickCurrent, rearTickLast;
int frontCounter=0,rearCounter=0;
bool firstFront=true, firstRear=true;

static int ticksPerRevolution=8;
static double l=20.0;//logitudinal length between axles in m
static double frontRadius=0.02; //front outer wheel radius in m
static double rearRadius=0.03; //rear outer wheel radius in m

double frontWheelCircumference=2*frontRadius*M_PI; //in m
double rearWheelCircumference=2*rearRadius*M_PI; //in m
double rearSpeed, frontSpeed; //in m/s
double sigma; //virtual steering angle

double x, xdot, y, ydot,yaw, yawdot, dt;

void updatePosition(){ // dead reckoning
    xdot=cos(yaw)*rearSpeed;
    ydot=sin(yaw)*rearSpeed;
    yawdot=1/l*tan(sigma)*rearSpeed;
    
    x=x+xdot*dt;
    y=y+ydot*dt;
    yaw=yaw+yawdot*dt;
}

double getDistance(){
    return rearCounter/(double)ticksPerRevolution*rearWheelCircumference;
}

double getFrontSpeed(){
    if (firstFront) {
        return 0;
    } else {
        return (frontWheelCircumference/ticksPerRevolution)/(frontTickCurrent-frontTickLast);
    }
}

double getRearSpeed(){
    if (firstRear) {
        return 0;
    } else {
        return (rearWheelCircumference/ticksPerRevolution)/(rearTickCurrent-rearTickLast);
    }
}

void frontEncoderAlert(int pin, int level, uint32_t tick)
{
    frontTickLast=frontTickCurrent;
    frontTickCurrent=tick;
    frontCounter++;
    firstFront=false;
}

void rearEncoderAlert(int pin, int level, uint32_t tick)
{
    //fixme in the beginning it get between 6 -12 tics by chance
    rearTickLast=rearTickCurrent;
    rearTickCurrent=tick;
    rearCounter++;
    printf("Current speed :%f m/s \n",getRearSpeed());
    printf("Total tics:%d \n",rearCounter);
    printf("Total distance :%f m \n",getDistance());
    firstRear=false;
}

int main(int argc, char *argv[])
{
    version = gpioInitialise();
    if (version >= 0)
    {
        gpioSetMode(frontEncoder, PI_INPUT);
        gpioSetMode(rearEncoder, PI_INPUT);
        gpioSetAlertFunc(frontEncoder, frontEncoderAlert);
        gpioSetAlertFunc(rearEncoder, rearEncoderAlert);
        
        //init ros stuff
        ros::init(argc, argv, "legocar_odometry");
        ros::NodeHandle n;
        ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 1000);
        nav_msgs::Odometry odom;
                
        int count = 0;
        ros::Rate r(1.0);
        while (ros::ok())
            
        {
            ros::spinOnce();
            
            geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(0.0);
            
            odom.header.stamp = ros::Time::now();
            odom.header.frame_id = "odom";
            
            //set the position
            odom.pose.pose.position.x = getDistance();
            odom.pose.pose.position.y = 0.0;
            odom.pose.pose.position.z = 0.0;
            odom.pose.pose.orientation = odom_quat;
            
            //set the velocity
            odom.child_frame_id = "base_link";
            odom.twist.twist.linear.x = getRearSpeed();
            odom.twist.twist.linear.y = 0.0;
            odom.twist.twist.angular.z = 0.0;
            
            //publish the message
            odom_pub.publish(odom);
            
            ++count;
            r.sleep();
        }

    }
    gpioTerminate();
    return 0;
}