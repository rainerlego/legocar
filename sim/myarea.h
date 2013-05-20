#ifndef GTKMM_EXAMPLE_MYAREA_H
#define GTKMM_EXAMPLE_MYAREA_H

#include <gtkmm-3.0/gtkmm/drawingarea.h>
#include "vectors.h"
#include "camera.h"
#include "car.h"
#include "map.h"
#include "sensor.h"

class MyArea : public Gtk::DrawingArea
{
public:
  MyArea();
  virtual ~MyArea();
  Camera myCamera;
  Car myCar;
  Map myMap;
  Sensor mySensor;

protected:

  bool key_left;
  bool key_right;
  bool key_up;
  bool key_down;

  double time;

  //Override default signal handler:
  virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
  void draw_car(const Cairo::RefPtr<Cairo::Context>& cr);
  void draw_map(const Cairo::RefPtr<Cairo::Context>& cr);
  void draw_sensordata(const Cairo::RefPtr<Cairo::Context>& cr);

  bool 	on_button_press_event (GdkEventButton*event);
  bool 	on_key_press_event (GdkEventKey*event);
  bool 	on_key_release_event (GdkEventKey*event);
  bool 	on_scroll_event (GdkEventScroll*event);

  bool on_timeout2(int i);
  bool on_timeout(int i);
};

#endif // GTKMM_EXAMPLE_MYAREA_H
