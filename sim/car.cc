#include "car.h"
#include <cmath>
#include <iostream>

Car::Car(){
  pos_back.x =3.0*m;
  pos_back.y =3.0*m;
  steering = M_PI/2;
  length = 0.4*m;
  speed = 0.0*m/s;
  width = 0.2*m;
  direction.x = 0.9;
  direction.y = 0.0;
  direction.norm();
  friction_coef = 2.0/s;
  accel = 0.0*m/s/s;
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
  double e = M_PI/400;
  //http://www.wolframalpha.com/input/?i=integrate+%28%28a*x%2Bv%29%2F%281%2Br*x%29+dx%29
  double range = ( (friction_coef*speed - accel)*log(friction_coef * time + 1) + accel*friction_coef*time )/ (friction_coef*friction_coef);
    //std::cout << " accel: " << accel <<" m/s^2\n";
    //std::cout << " steering: " << steering/(M_PI/2) <<"\n";
  if(steering<M_PI/2+e && steering > M_PI/2-e ){
    //std::cout << " steering angular too small!!\n";
    pos_back = pos_back + (direction * range);
  }else{
  
    vect2 M = get_center_of_rotation();

    vect2 C = (pos_back - M).get_rotated(range/getR());

    pos_back = M + C;
    
    direction = direction.get_rotated(range/getR());
  }
  double speednew = speed + accel*time -speed*time*friction_coef;
  speed = speednew;
  
  

}

vect2 Car::get_center_of_rotation(void){
  if(steering<M_PI/2){
    return direction.getOrth() * (-getR()) + pos_back;
  }else if(steering >M_PI/2){
    return direction.getOrth() * (-getR()) + pos_back;
  }else{
    vect2 res;
    res.x = 0;
    res.y = 0;
    return res;
  }
}

double Car::getR(void){
  return tan(steering)*length;
}


