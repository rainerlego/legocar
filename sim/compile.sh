#!/bin/bash
#c++ `pkg-config --cflags --libs gtkmm-3.0` -lm -lpthread main.cc global.c tcpserver.c car.cc camera.cc myarea.cc vectors.cc map.cc sensor.cc
make -j$1
