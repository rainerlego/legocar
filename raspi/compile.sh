#!/bin/bash
gcc -o wiitcp.exe servo.c wiimote.c tcpserver.c main.c -lpthread -lcwiid
