#include "vectors.h"
#include <cmath>

vect2::vect2(){

}

vect2::~vect2(){

}

void vect2::norm(void){
  double abs = sqrt(x*x+y*y);
  x/=abs;
  y/=abs;
}
vect2 vect2::operator*(const double rhs){
  vect2 result;
  result.x = x*rhs;
  result.y = y*rhs;
  return result;
}
vect2 vect2::operator-(const vect2 rhs){
  vect2 result;
  result.x = -rhs.x + x;
  result.y = -rhs.y +y;
  return result;
}
vect2 vect2::operator+(const vect2 rhs){
  vect2 result;
  result.x = rhs.x + x;
  result.y = rhs.y +y;
  return result;
}

vect2 vect2::getOrth(){
  vect2 result;
  result.x = y;
  result.y = -x;
  result.norm();
  return result;
}

vect2 vect2::get_rotated(double angular){
  vect2 result;
  result.x = x *cos(angular) - y*sin(angular);
  result.y = x*sin(angular) + y*cos(angular);
  return result;
}
