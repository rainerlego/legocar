#include <stdint.h>
#include "config.h"

#define SERVO_PERM_TCP 0
#define SERVO_PERM_WII 1
#define SERVO_PERM_XBOX 2

int servo_init();
int servo_open();
void servo_close();
int servo_getperm ( int src, int port );
int servo_setservo ( uint8_t servoNr, uint16_t servoPos, int force, int src, int port );
int servo_setleds ( uint8_t onoff, uint8_t leds, int force, int src, int port );
void servo_ping();
void servo_testservos();
