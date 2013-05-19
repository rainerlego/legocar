#ifndef GTKMM_EXAMPLE_MYAREA_H
#define GTKMM_EXAMPLE_MYAREA_H

#include <gtkmm-3.0/gtkmm/drawingarea.h>
#include "vectors.h"
#include "camera.h"
#include "car.h"

class MyArea : public Gtk::DrawingArea
{
public:
  MyArea();
  virtual ~MyArea();
  Camera myCamera;
  Car myCar;

protected:
  //Override default signal handler:
  virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
  void draw_car(const Cairo::RefPtr<Cairo::Context>& cr);

  bool 	on_button_press_event (GdkEventButton*event);
  bool on_timeout(int i);
};

#endif // GTKMM_EXAMPLE_MYAREA_H
