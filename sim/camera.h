#ifndef CAMERA_H
#define CAMERA_H

#include "vectors.h"

class Camera{
public:
  double ppm; //pixel per meter
  Camera();
  ~Camera();
  vect2 pos; //position in meters
  vect2 transform_to_m (vect2 pos);
  vect2 transform (vect2 pos);
  vect2 get_center();
  double height;
  double width;
  void set_ppm(double v);
  void set_center(vect2 p);
  void set_geometry(double width, double height);
  double scale (double val);
  double scale_to_m (double val);
protected:
};


#endif

