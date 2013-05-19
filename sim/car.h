
#ifndef CAR_H
#define CAR_H
#include "units.h"
#include "vectors.h"

class Car
{
public:
  Car();
  ~Car();
  double length;
  double width;
  vect2 pos_back;
  double speed;
  double set_speed;
  double accel;
  double steering;
  vect2 direction;
  vect2 get_pos_front(void);
  vect2 get_pos_back(void);

  double friction_coef;

  void move(double time);

  double getR(void);
  vect2 get_center_of_rotation(void);
};

#endif // GTKMM_EXAMPLE_MYAREA_H
