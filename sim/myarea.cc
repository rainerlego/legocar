#include "myarea.h"
#include <cairomm/context.h>
#include "vectors.h"
#include <iostream>
#include "units.h"
#include <map>
#include <gtkmm.h>

MyArea::MyArea()
{
  add_events(Gdk::BUTTON_PRESS_MASK );
  add_events(Gdk::KEY_PRESS_MASK );
  sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*this,&MyArea::on_timeout), 0);
  sigc::connection myConnection = Glib::signal_timeout().connect(my_slot, 10);

}

bool MyArea::on_timeout(int i){
  //std::cout << " timer\n";
  //myCar.move(100*ms);
  //on_draw();
  myCar.move(10*ms);
  queue_draw();

  return true;
}

MyArea::~MyArea()
{
}

bool 	MyArea::on_button_press_event (GdkEventButton*event){

  std::cout << " button press\n";
  myCar.move(100*ms);
  queue_draw();
  return false;
}


bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  // coordinates for the center of the window
  int xc, yc;
  xc = width / 2;
  yc = height / 2;


  double scalelength = myCamera.scale(1.0*m);
  cr->set_source_rgb(0.4, 0.0, 0.0);
  cr->set_line_width(2.0);
  cr->move_to(10.0,10.0);
  cr->line_to(10.0+scalelength,10.0);
  cr->stroke();
  // draw red lines out from the center of the window
 
  

  draw_car(cr);

  return true;
}


void MyArea::draw_car(const Cairo::RefPtr<Cairo::Context>& cr){
  vect2 back = myCamera.transform(myCar.get_pos_back());
  vect2 front = myCamera.transform(myCar.get_pos_front());
  vect2 orth = (front-back).getOrth();
  cr->set_line_width(1.0);
  cr->set_source_rgb(0.0, 0.0, 0.6);
  cr->move_to(back.x, back.y);
  cr->line_to(front.x, front.y);
  cr->stroke();

  //std::cout << myCamera.scale(myCar.width) << "\n";

  vect2 tempv = back + orth*(myCamera.scale(myCar.width)/2);
  vect2 tempv2 = back - orth*(myCamera.scale(myCar.width)/2);
  cr->move_to(tempv.x, tempv.y);
  cr->line_to(tempv2.x, tempv2.y);
  cr->move_to(tempv2.x, tempv2.y);
  cr->line_to(front.x, front.y);
  cr->move_to(tempv.x, tempv.y);
  cr->line_to(front.x, front.y);

  cr->stroke();
  
}
