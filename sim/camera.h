#ifndef CAMERA_H
#define CAMERA_H

#include "vectors.h"

class Camera{
  double ppm; //pixel per meter
public:
  Camera();
  ~Camera();
  vect2 pos;
  vect2 transform (vect2 pos);
protected:
};


#endif

