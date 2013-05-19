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
vect2 vect2::operator+(const vect2 rhs){
  vect2 result;
  result.x = rhs.x + x;
  result.y = rhs.y +y;
  return result;
}
