// Ultrasonic rangefinder SRF02 via i2c on raspi

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ros/ros.h"
#include "sensor_msgs/Range.h"


int main(int argc, char **argv)
{
    
    //init i2c stuff
    int fd;														// File descrition
	char fileName[] = "/dev/i2c-1";								// Name of the port we will be using
	int  address = 0x70;										// Address of the SRF02 shifted right one bit
	unsigned char buf[10];										// Buffer for data being read/ written on the i2c bus
	
	if ((fd = open(fileName, O_RDWR)) < 0) {					// Open port for reading and writing
		printf("Failed to open i2c port\n");
		exit(1);
	}
	
	if (ioctl(fd, I2C_SLAVE, address) < 0) {					// Set the port options and set the address of the device we wish to speak to
		printf("Unable to get bus access to talk to slave\n");
		exit(1);
	}
    
    //init ros stuff
    ros::init(argc, argv, "legocar_sonar");
    ros::NodeHandle n;
    ros::Publisher range_pub = n.advertise<sensor_msgs::Range>("sonar", 1000);
    
    //setup a sensor message
    sensor_msgs::Range msg;
    msg.radiation_type=0;
    msg.field_of_view=0.6;
    msg.min_range=0.17;
    msg.max_range=6.0;

    int count = 0;
    while (ros::ok())
    {
        buf[0] = 0;													// Commands for performing a ranging
        buf[1] = 81;                                                // Range in cm
        
        if ((write(fd, buf, 2)) != 2) {								// Write commands to the i2c port
            printf("Error writing to i2c slave\n");
            exit(1);
        }
        
        usleep(750000);												// This sleep waits for the ping to come back
        
        buf[0] = 0;													// This is the register we wish to read from
        
        if ((write(fd, buf, 1)) != 1) {								// Send the register to read from
            printf("Error writing to i2c slave\n");
            exit(1);
        }
        
        if (read(fd, buf, 4) != 4) {								// Read back data into buf[]
            printf("Unable to read from slave\n");
            exit(1);
        }
        else {
            unsigned char highByte = buf[2];
            unsigned char lowByte = buf[3];
            unsigned int result = (highByte <<8) + lowByte;			// Calculate range as a word value
            printf("Range was: %u\n",result);
    
            msg.range = static_cast< float >(result);
            ROS_INFO("Sonar reading at %f", msg.range);
            range_pub.publish(msg);
        }
        

        ros::spinOnce();
        ++count;
    }
    
    return 0;
}