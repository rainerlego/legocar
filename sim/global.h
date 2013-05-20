#ifndef GLOBAL_H
#define GLOBAL_H

#include "myarea.h"

#define CTRL_USER 0
#define CTRL_TCP 1
extern int global_ctrl;
extern MyArea * global_area;
void global_ctrl_switch();
#endif
