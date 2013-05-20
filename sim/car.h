
#ifndef CAR_H
#define CAR_H
#include "units.h"
#include "vectors.h"

#define steer_max (M_PI/8.0)
#define accel_max (10.0*m/s/s)
#define steer_mid (M_PI/2.0)

class Car
{
public:
  Car();
  ~Car();
  double length;
  double width;
  vect2 pos_back;
  double speed;
  double accel;
  double steering;
  vect2 direction;
  vect2 get_pos_front(void);
  vect2 get_pos_back(void);

  double friction_coef;

  void move(double time);

  double getR(void);
  vect2 get_center_of_rotation(void);

	void set_steering_servo ( int servo );
	void set_accel_servo ( int servo );
};

#endif // GTKMM_EXAMPLE_MYAREA_H
