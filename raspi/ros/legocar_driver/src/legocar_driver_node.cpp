#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ros/ros.h>
#include <ackermann_msgs/AckermannDrive.h>

#define TWI_PREAMBLE 0xff
#define CMD_SERVO 0
#define CMD_LED 1
#define SERVOBOARD_ADDR 0x02
#define SERVOBOARD_I2C_CHARDEV "/dev/i2c-0"

#define LOWBYTE(v) ((unsigned char) (v))
#define HIGHBYTE(v) ((unsigned char)(((unsigned int) (v)) >> 8))


int fd;
pthread_mutex_t servo_mutex;

int servoboard_init()
{
    pthread_mutex_init ( &servo_mutex, NULL );
    return 0;
}

int servoboard_open()
{
    if ((fd = open(SERVOBOARD_I2C_CHARDEV, O_RDWR)) < 0) {
        printf("Failed to open i2c port\n");
        return -1;
    }
    
    if (ioctl(fd, I2C_SLAVE, SERVOBOARD_ADDR) < 0) {
        printf("Unable to get bus access to talk to slave\n");
        return -1;
    }
    
    return 0;
}

void servoboard_close()
{
    close(fd);
}

int servoboard_setservo ( uint8_t servoNr, uint16_t servoPos )
{
    unsigned char buf[4];
    
    if ( (servoPos < 0) || (servoPos > 8000 ) ) {
        printf ( "E: servoPos (%d) out of range\n", servoPos );
        return -1;
    }
    if ( ( servoNr < 0) || ( servoNr > 7) ) {
        printf ( "E: servoNr (%d) out of range\n", servoNr );
        return -1;
    }
    
    buf[0] = TWI_PREAMBLE;
    buf[1] = (CMD_SERVO<<4) | (servoNr & 0xf);
    buf[2] = HIGHBYTE(servoPos);
    buf[3] = LOWBYTE(servoPos);
    
    pthread_mutex_lock ( &servo_mutex );
    if ((write(fd, buf, 4)) != 4) {
        printf("E: Could not write specified amount of bytes to i2c chardev\n");
        pthread_mutex_unlock ( &servo_mutex );
        return -1;
    }
    pthread_mutex_unlock ( &servo_mutex );
    
    return 0;
}

int servoboard_setleds ( uint8_t onoff, uint8_t leds )
{
    unsigned char buf[2];
    
    buf[0] = TWI_PREAMBLE;
    buf[1] = 0;
    
    buf[1] = leds & ((1<<2)|(1<<1)|(1<<0));
    if ( onoff )
        buf[1] |= (1<<3);
    
    pthread_mutex_lock ( &servo_mutex );
    if ((write(fd, buf, 2)) != 2) {
        printf("E: Could not write specified amount of bytes to i2c chardev\n");
        pthread_mutex_unlock ( &servo_mutex );
        return -1;
    }
    pthread_mutex_unlock ( &servo_mutex );
    
    return 0;
}

int servotickForAngle(float angle){// angle is in radians
    return 4000+(int)(angle*5092.95818152);    //max angle between 0.785398163 and -0.785398163 rad == +45 -45 degrees, positive yaw is to the left
}

int servotickForSpeed(float speed){     //speed between 2600 and 8000 in 0-100%

    return 2600+(int)(speed*54);
}

void msgCallback(const ackermann_msgs::AckermannDrive::ConstPtr& msg)
{
    ROS_INFO("Speed %f at steering angle %f ",msg->speed, msg->steering_angle);
    
    //set steering
    servoboard_setservo(0,servotickForAngle(msg->steering_angle));
    //set speed
    servoboard_setservo(1,servotickForSpeed(msg->speed));
}

int main(int argc, char **argv)
{
    servoboard_init();
    servoboard_open();
    ros::init(argc, argv, "legocar_driver_node");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("cmd_vel", 1000, msgCallback);
    ros::spin();
    servoboard_close();
    return 0;
}