#include "sensor.h"
#include <cmath>
#include "vectors.h"
#include "units.h"
#include <iostream>

Sensor::Sensor(){
  angular = 2.0*M_PI*45.0/360.0/2.0;
  range = 7.0*m;

}

Sensor::~Sensor(){

}

void Sensor::scan(){
  int i;
  //std::cout << "scan ... \n";
  for(i=0;i<myMap->ocount;i++){
    vect2 to_obst = myMap->obstacles[i].pos - myCar->get_pos_front();
    to_obst = to_obst - to_obst.get_norm()*myMap->obstacles[i].r;

    vect2 obst_left = to_obst - to_obst.getOrth()*myMap->obstacles[i].r;
    vect2 obst_right = to_obst + to_obst.getOrth()*myMap->obstacles[i].r;

    double angular_right = acos(myCar->direction * obst_left.get_norm());
    double angular_left = acos(myCar->direction * obst_right.get_norm());
    std::cout << "scan obj " << i << " ar = " << angular_right*360.0/2.0/M_PI << " al = " << angular_left*360.0/2.0/M_PI << " \n";
    if(std::abs(angular_right) < angular || std::abs(angular_left) < angular ||
        (((angular_left < -angular  && angular_right > angular) || (angular_left>angular && angular_right < -angular) ) && (std::abs(angular_left)+std::abs(angular_right) < M_PI ) ) ){
      std::cout << "OBJECT NR " << i << " IN RANGE! \n";
    }


    

  }

}
