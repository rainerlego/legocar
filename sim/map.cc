#include "map.h"
#include "units.h"

Obstacle::Obstacle(){

}
Obstacle::~Obstacle(){

}

Map::Map(){

  obstacles[0].pos.x = 1.0*m;
  obstacles[0].pos.y = 1.0*m;
  obstacles[0].r = 10.0*cm;

  obstacles[1].pos.x = 5.0*m;
  obstacles[1].pos.y = 5.0*m;
  obstacles[1].r = 18.0*cm;

  obstacles[2].pos.x = 8.0*m;
  obstacles[2].pos.y = 3.0*m;
  obstacles[2].r = 7.0*cm;

  obstacles[3].pos.x = 2.0*m;
  obstacles[3].pos.y = 4.0*m;
  obstacles[3].r = 20.0*cm;

  obstacles[4].pos.x = 5.0*m;
  obstacles[4].pos.y = 2.0*m;
  obstacles[4].r = 50.0*cm;
  
  ocount = 5;

}

Map::~Map(){
}
