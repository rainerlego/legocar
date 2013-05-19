
#ifndef VECTOR_H
#define VECTOR_H

class vect2 {
public:
  vect2();
  ~vect2();
  double x;
  double y;
  void norm(void);
  vect2 operator*(const double rhs);
  vect2 operator+(const vect2 rhs);
  vect2 operator-(const vect2 rhs);
  vect2 getOrth();
  vect2 get_rotated(double angular);
};

#endif
