#include<stdio.h>
#include<string.h>  //strlen
#include<stdlib.h>  //strlen

#include "global.h"

int global_ctrl;

void global_ctrl_switch()
{
  if (global_ctrl == CTRL_USER)
  {
    //printf ( "switching to tcp control\n" );
    global_ctrl = CTRL_TCP;
  } else {
    //printf ( "switching to user control\n" );
    global_ctrl = CTRL_USER;
  }
}
