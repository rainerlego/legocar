#!/bin/bash
c++ `pkg-config --cflags --libs gtkmm-3.0` -lm main.cc car.cc camera.cc myarea.cc vectors.cc
