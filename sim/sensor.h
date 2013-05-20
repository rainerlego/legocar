#ifndef Sensor_H
#define Sensor_H

#include "car.h"
#include "map.h"

class Sensor{
public:
  Sensor();
  ~Sensor();
  double angular;
  double range;
  int angular_resolution;
  Car * myCar;
  Map * myMap;
  double data[100];

  void scan();
private:
  double angular_per_slot;
};

#endif
