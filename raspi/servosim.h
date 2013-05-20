#include <stdint.h>

int servosim_open();
void servosim_close();
int servosim_setservo ( uint8_t servoNr, uint16_t servoPos );
int servosim_setleds ( uint8_t onoff, uint8_t leds );
