#include<stdio.h>
#include<string.h>  //strlen
#include<stdlib.h>  //strlen

#include "global.h"
#include "myarea.h"

int global_ctrl;
MyArea * global_area;

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
