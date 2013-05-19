#include "myarea.h"
#include <cairomm/context.h>
#include "vectors.h"
#include <iostream>
#include "units.h"
#include <map>
#include <gtkmm.h>
#include <string>
#include <sstream>

MyArea::MyArea()
{
  add_events(Gdk::BUTTON_PRESS_MASK );
  add_events(Gdk::KEY_PRESS_MASK );
  add_events(Gdk::KEY_RELEASE_MASK );
  set_can_focus(true);
  sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*this,&MyArea::on_timeout), 0);
  sigc::connection myConnection = Glib::signal_timeout().connect(my_slot, 10);


  key_left = false;
  key_right = false;
  key_up = false;
  key_down = false;

  time = 0;
}

bool MyArea::on_timeout(int i){
  //std::cout << " timer\n";
  //myCar.move(100*ms);
  //on_draw();
  //
  time += 10*ms;
  double steeringstep = M_PI/500.0;
  double max_steer = M_PI/8.0;
  double max_accel = 10.0*m/s/s;
  double accelstep = 0.05;
  if(key_up){
    myCar.accel+=accelstep;
    if(myCar.accel > max_accel){
      myCar.accel = max_accel;
    }
  }
  if(key_down){
    myCar.accel-=accelstep;
    if(myCar.accel < -max_accel){
      myCar.accel = -max_accel;
    }
  }
  if(key_left){
    myCar.steering+=steeringstep;
    if(myCar.steering > M_PI/2.0+ max_steer){
      myCar.steering = M_PI/2.0 + max_steer;
    }
  }
  if(key_right){
    myCar.steering-=steeringstep;
    if(myCar.steering < M_PI/2.0-max_steer){
      myCar.steering = M_PI/2.0 - max_steer;
    }
  }
  myCar.move(10*ms);
  queue_draw();

  return true;
}

MyArea::~MyArea()
{
}

bool 	MyArea::on_key_release_event (GdkEventKey*event){

  //std::cout << " key release\n";
  switch(event->keyval){
    case 65363:
      key_right = false;
      break;
    case 65361:
      key_left = false;
      break;
    case 65362:
      key_up = false;
      break;
    case 65364:
      key_down = false;
      break;
  }
  return false;
}

bool 	MyArea::on_key_press_event (GdkEventKey*event){

  //std::cout << " key press " << event->keyval <<"\n";
  switch(event->keyval){
    case 65363:
      key_right = true;
      break;
    case 65361:
      key_left = true;
      break;
    case 65362:
      key_up = true;
      break;
    case 65364:
      key_down = true;
      break;
  }
  return false;
}

bool 	MyArea::on_button_press_event (GdkEventButton*event){

  std::cout << " button press\n";
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
  cr->move_to(10.0,height-10.0);
  cr->line_to(10.0+scalelength,height - 10.0);
  cr->stroke();
  // draw red lines out from the center of the window
 
  Cairo::RefPtr<Cairo::ToyFontFace> font =
    Cairo::ToyFontFace::create("Bitstream Charter",
                               Cairo::FONT_SLANT_NORMAL,
                               Cairo::FONT_WEIGHT_NORMAL);
  cr->set_font_face(font);
  cr->set_font_size(8.0);

  cr->move_to(10.0,10.0);
  std::stringstream sspeed;
  sspeed << "speed: " << myCar.speed/(km/h) << "km/h = " << myCar.speed/(m/s) << " m/s";
  cr->show_text(sspeed.str());

  cr->move_to(10.0,20.0);
  std::stringstream sacc;
  sacc << "engine acceleration: " << myCar.accel/(m/s/s) << " m/s^2";
  cr->show_text(sacc.str());

  cr->move_to(10.0,30.0);
  std::stringstream sheading;
  sheading << "steering: " << (myCar.steering-M_PI/2.0)*360.0/(2.0*M_PI) << " deg";
  cr->show_text(sheading.str());

  cr->move_to(10.0,40.0);
  std::stringstream stime;
  stime << "simulation time: " << time/s << " s";
  cr->show_text(stime.str());

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

  cr->set_source_rgb(0.6, 0.0, 0.0);
  vect2 frontaxis = myCar.direction.get_rotated(-myCar.steering);
  vect2 leftwheel = front + frontaxis*myCamera.scale(myCar.width/2.0);
  vect2 rightwheel = front - frontaxis*myCamera.scale(myCar.width/2.0);
  cr->move_to(leftwheel.x, leftwheel.y);
  cr->line_to(rightwheel.x, rightwheel.y);

  vect2 frontwheel = myCar.direction.get_rotated(- (myCar.steering + M_PI/2.0));
  frontwheel = frontwheel * myCamera.scale(5*cm);
  vect2 fwf = leftwheel + frontwheel;
  vect2 fwb = leftwheel - frontwheel;
  cr->move_to(fwf.x, fwf.y);
  cr->line_to(fwb.x, fwb.y);
  fwf = rightwheel + frontwheel;
  fwb = rightwheel - frontwheel;
  cr->move_to(fwf.x, fwf.y);
  cr->line_to(fwb.x, fwb.y);
  
  //vect2 M = myCamera.transform(myCar.get_center_of_rotation());

  //cr->move_to(back.x, back.y);
  //cr->line_to(M.x, M.y);

  cr->stroke();
  
}
