/*
 * main.c
 *
 *  Created on: Apr 16, 2013
 *      Author: rainer
 */


#include <stdio.h>
#include <system.h>
#include "altera_avalon_pio_regs.h"
int main() {
  printf("Hello World!\n");
  int count = 0;
  int delay;
  while (1) {
    IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE, count & 0xff);
    delay = 0;
    while (delay < 200000) {
      delay++;
    }
    //printf("Hello!\n");
    count++;
  }
  return 0;
}
