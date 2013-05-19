#include "camera.h"
#include <iostream>

Camera::Camera(){
  pos.x = 0.0; 
  pos.y = 0.0; 
  ppm = 80;
}
Camera::~Camera(){

}


vect2 Camera::transform (vect2 point){
  vect2 result;
  result.x = point.x*ppm-pos.x*ppm;
  result.y = point.y*ppm-pos.y*ppm;
  return result;
}

void Camera::set_geometry(double mwidth, double mheight){
  height = mheight;
  width = mwidth;
}

vect2 Camera::get_center(){
  vect2 result;
  result.x = pos.x + scale_to_m(width/2.0);
  result.y = pos.y + scale_to_m(height/2.0);
  //std::cout << " pos: " << pos.x <<" " << pos.y  <<"\n";
  //std::cout << " get center: " << result.x <<" " << result.y  <<"\n";
  return result;
}

void Camera::set_center(vect2 p){
  pos.x = p.x - scale_to_m(width/2.0);
  pos.y = p.y - scale_to_m(height/2.0);
}

void Camera::set_ppm(double v){
  vect2 tc = get_center();
  ppm = v;
  set_center(tc);
}

double Camera::scale_to_m (double val){
  return val/ppm;
}

double Camera::scale (double val){
  return ppm*val;
}
