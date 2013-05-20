#!/bin/bash
#gcc -o wiitcp.exe servo.c servoboard.c servosim.c wiimote.c tcpserver.c main.c -lpthread -lcwiid
make -j$1
