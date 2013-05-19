#include "car.h"
#include <cmath>

Car::Car(){
  pos_back.x =10.0*m;
  pos_back.y =10.0*m;
  steering = 2;
  length = 0.5*m;
  speed = 0.3*m/s;
  direction.x = 0.5;
  direction.y = 0.3;
  direction.norm();
}

Car::~Car(){

}

vect2 Car::get_pos_back(void){

  return pos_back;
}
vect2 Car::get_pos_front(void){
  vect2 result = pos_back + (direction*length);
  return result;
}
void Car::move(double time){

}

double Car::getR(void){
  return tan(steering)*length;
}


