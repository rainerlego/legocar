#include "camera.h"

Camera::Camera(){
  pos.x = 0.0; 
  pos.y = 0.0; 
  ppm = 20;
}
Camera::~Camera(){

}


vect2 Camera::transform (vect2 point){
  vect2 result;
  result.x = point.x*ppm-pos.x;
  result.y = point.y*ppm-pos.y;
  return result;
}
