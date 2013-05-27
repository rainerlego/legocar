#include <stdint.h>
#include "config.h"

int servo_init();
int servo_open();
void servo_close();
int servo_setservo ( uint8_t servoNr, uint16_t servoPos, int force );
int servo_setleds ( uint8_t onoff, uint8_t leds, int force );
void servo_testservos();
