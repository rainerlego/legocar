
#define TWI_PREAMBLE 0xff
#define CMD_SERVO 0
#define SERVOBOARD_ADDR 0x02
#define SERVOBOARD_I2C_CHARDEV "/dev/i2c-0"

#define LOWBYTE(v) ((unsigned char) (v))
#define HIGHBYTE(v) ((unsigned char)(((unsigned int) (v)) >> 8))

int servoboard_open ( char * chardev, int addr );
void servoboard_close();
int servoboard_setservo ( uint8_t servoNr, uint16_t servoPos );
void servoboard_testservos();
