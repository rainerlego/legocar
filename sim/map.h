#ifndef MAP_H
#define MAP_H
#include "vectors.h"

class Obstacle{
public:
  Obstacle();
  ~Obstacle();
  double r;
  vect2 pos;
};

class Map{
public:
  Map();
  ~Map();
  Obstacle obstacles[20];
  int ocount;

  
};

#endif
