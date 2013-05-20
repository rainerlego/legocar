#include "sensor.h"
#include <cmath>
#include "vectors.h"
#include "units.h"
#include <iostream>

Sensor::Sensor(){
  angular = 2.0*M_PI*45.0/360.0/2.0;
  range = 5.0*m;
  angular_resolution = 100;

  angular_per_slot = angular*2.0/(double)angular_resolution;
}

Sensor::~Sensor(){

}

void Sensor::scan(){
  int i;
  //std::cout << "scan ... \n";
  for(i=0;i<angular_resolution;i++){
    data[i] = range;
  }
  for(i=0;i<myMap->ocount;i++){
    vect2 to_obst = myMap->obstacles[i].pos - myCar->get_pos_front();
    to_obst = to_obst - to_obst.get_norm()*myMap->obstacles[i].r;

    vect2 obst_left = to_obst - to_obst.getOrth()*myMap->obstacles[i].r;
    vect2 obst_right = to_obst + to_obst.getOrth()*myMap->obstacles[i].r;

    vect2 oln = obst_left.get_norm();
    vect2 orn = obst_right.get_norm();
    vect2 dir = myCar->direction;
    double angular_left = acos(dir * oln);
    if(dir.x*oln.y - dir.y*oln.x < 0){
      angular_left = -angular_left;
    }
    double angular_right = acos(dir * orn);
    if(dir.x*orn.y - dir.y*orn.x < 0){
      angular_right = -angular_right;
    }
    //std::cout << "scan obj " << i << " ar = " << angular_right*360.0/2.0/M_PI << " al = " << angular_left*360.0/2.0/M_PI << " \n";
    if( (std::abs(angular_right) < angular || std::abs(angular_left) < angular ||
        (((angular_left < -angular  && angular_right > angular) || (angular_left>angular && angular_right < -angular) ) && (std::abs(angular_left)+std::abs(angular_right) < M_PI ) )) &&
        to_obst.abs() < range
        ){
      //std::cout << "OBJECT NR " << i << " IN RANGE! \n";
      double ang_start;
      double distance_start;
      double ang_stop;
      double distance_stop;
      if(angular_left < angular_right){
        ang_start = angular_left + angular;
        distance_start = obst_left.abs();
        ang_stop = angular_right + angular;
        distance_stop = obst_right.abs();
      }else{
        ang_start = angular_right + angular;
        distance_start = obst_right.abs();
        ang_stop = angular_left + angular;
        distance_stop = obst_left.abs();
      }
      int ii;
      //std::cout << "ang_start: " << ang_start << " ang_stop: " << ang_stop << "\n";
      //std::cout << "ang_start: " << (int)(ang_start/angular_per_slot) << " ang_stop: " << (int)(ang_stop/angular_per_slot) << "\n";
      for(ii= (int) (ang_start/angular_per_slot); ii< (int)(ang_stop/angular_per_slot) ; ii++){
        if(ii>=0 && ii<angular_resolution){
          
          double value = distance_start + (distance_stop - distance_start) * ((double)ii * angular_per_slot )/(ang_stop - ang_start);
          //std::cout << "i = "
          if(value< data[ii]){
            data[ii] = value;
          }
        }
      }


    }


    

  }

}
