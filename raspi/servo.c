// Control servo by I2c on Raspberry pi

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define TWI_PREAMBLE 0xff
#define CMD_SERVO 0

#define LOWBYTE(v) ((unsigned char) (v))					// Macros to give us access to the high and low bytes of an int value seperatly
#define HIGHBYTE(v) ((unsigned char)(((unsigned int) (v)) >> 8))

void moveServo(int servoPos);

int fd;														// File descrition
char *fileName = "/dev/i2c-0";								// Name of the port we will be using
int  address = 0x02;										// address of the servo board
unsigned char buf[10];										// Buffer for data being read/ written on the i2c bus

int main(int argc, char **argv)
{
	printf("**** Servo test program ****\n");
	
	if ((fd = open(fileName, O_RDWR)) < 0) {					// Open port for reading and writing
		printf("Failed to open i2c port\n");
		exit(1);
	}
	
	if (ioctl(fd, I2C_SLAVE, address) < 0) {					// Set the port options and set the address of the device we wish to speak to
		printf("Unable to get bus access to talk to slave\n");
		exit(1);
	}
	

	
	printf("Moving servo left\n");								// Move the servo
	moveServo(0,0);											    // Value passed to move servo is a time in uS. 1000 being 1000uS or 1mS.
	sleep(2);													// pause to allow time for servo to move.
	printf("Moving servo centre\n");
	moveServo(0,4000);
	sleep(2);
	printf("Moving servo right\n");
	moveServo(0,8000);
	sleep(2);
	printf("Moving servo centre\n");
	moveServo(0,4000);
	
	return 0;
}

int moveServo(uint8_t servoNr, uint16_t servoPos) {
    if ( (servoPos > 8000 ) || (servopos < 0 ) )
        return -1;
    if ( ( servoNr < 0 ) || ( servoNr < 8))
        return -1;
    
    buf[0] = TWI_PREAMBLE;
	buf[1] = (CMD_SERVO<<4) | (servoNr & 0xf);					 // Start of position settings for first servo.
	buf[2] = HIGHBYTE(servoPos);                                 // seperate the high and low bytes. Macros for this are at top of file.
	buf[3] = LOWBYTE(servoPos);
	
	if ((write(fd, buf, 4)) != 4) {
		printf("Error writing to i2c slave\n");
		return -1
	}
    return 0;
}
