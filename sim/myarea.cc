#include "myarea.h"
#include <cairomm/context.h>
#include "vectors.h"
#include <iostream>
#include "units.h"
#include <map>
#include <gtkmm.h>
#include <string>
#include <sstream>

#include "global.h"

MyArea::MyArea()
{
  add_events(Gdk::SCROLL_MASK );
  add_events(Gdk::BUTTON_PRESS_MASK );
  add_events(Gdk::KEY_PRESS_MASK );
  add_events(Gdk::KEY_RELEASE_MASK );
  set_can_focus(true);
  sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*this,&MyArea::on_timeout), 0);
  sigc::connection myConnection = Glib::signal_timeout().connect(my_slot, 10);
  sigc::slot<bool> my_slot2 = sigc::bind(sigc::mem_fun(*this,&MyArea::on_timeout2), 0);
  sigc::connection myConnection2 = Glib::signal_timeout().connect(my_slot2, 100);


  //attach sensor to car:
  mySensor.myMap = &myMap;
  mySensor.myCar = &myCar;


  key_left = false;
  key_right = false;
  key_up = false;
  key_down = false;

  time = 0;

  leds = 0;
}

bool MyArea::on_timeout2(int i){
  mySensor.scan();

  
  return true;
}
bool MyArea::on_timeout(int i){
  //std::cout << " timer\n";
  //myCar.move(100*ms);
  //on_draw();
  //
  time += 10*ms;
  double steeringstep = M_PI/500.0;
  double accelstep = 0.05;

  if ( global_ctrl == CTRL_USER )
  {
    if(key_up){
      myCar.accel+=accelstep;
      if(myCar.accel > accel_max){
        myCar.accel = accel_max;
      }
    }
    if(key_down){
      myCar.accel-=accelstep;
      if(myCar.accel < -accel_max){
        myCar.accel = -accel_max;
      }
    }
    if(key_left){
      myCar.steering+=steeringstep;
      if(myCar.steering > steer_mid + steer_max){
        myCar.steering = steer_mid + steer_max;
      }
    }
    if(key_right){
      myCar.steering-=steeringstep;
      if(myCar.steering < steer_mid - steer_max){
        myCar.steering = steer_mid - steer_max;
      }
    }
  }
  myCar.move(10*ms);
  queue_draw();

  return true;
}

MyArea::~MyArea()
{
}

bool   MyArea::on_key_release_event (GdkEventKey*event){

  //printf("key up: %d\n", event->keyval);
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
    case 65307: //esc -> reset
      myCar.speed = 0.0;
      myCar.accel = 0.0;
      myCar.steering = M_PI/2.0;
      myCar.pos_back.x = 3.0*m;
      myCar.pos_back.y = 3.0*m;
      myCar.direction.x = 0.9;
      myCar.direction.y = 0.0;
      myCar.direction.norm();
      break;
    case 32: //spacebar -> motor off
      myCar.accel = 0.0;
      break;
    case 119: //w
      myCamera.pos.y -= myCamera.scale_to_m(myCamera.height/2.0);
      break;
    case 115: //s
      myCamera.pos.y += myCamera.scale_to_m(myCamera.height/2.0);
      break;
    case 97: //a
      myCamera.pos.x -= myCamera.scale_to_m(myCamera.width/2.0);
      break;
    case 100: //d
      myCamera.pos.x += myCamera.scale_to_m(myCamera.width/2.0);
      break;
    case 99: //c
      global_ctrl_switch();
      break;
  }
  return false;
}

bool   MyArea::on_key_press_event (GdkEventKey*event){

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

bool   MyArea::on_scroll_event (GdkEventScroll*event){

  //std::cout << " scroll " << event->direction << "\n";
  if(event->direction == 1){
    myCamera.set_ppm(myCamera.ppm/1.5);
  }else{
    myCamera.set_ppm(myCamera.ppm*1.5);
  }
  return false;
}

bool   MyArea::on_button_press_event (GdkEventButton*event){

  std::cout << " button press " << event->button << "\n";
  return false;
}


bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();
  myCamera.set_geometry(width, height);
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

  cr->move_to(10.0,50.0);
  std::stringstream sctrl;
  if ( global_ctrl == CTRL_USER )
  {
    cr->set_source_rgb(0.4, 0.0, 0.0);
    sctrl << "Control: user";
  } else {
    cr->set_source_rgb(0.0, 0.0, 0.4);
    sctrl << "Control: TCP";
  }
  cr->show_text(sctrl.str());

  cr->move_to(10.0,60.0);
  cr->set_source_rgb(0.4, 0.0, 0.0);
  std::stringstream sleds;
  sleds << "Leds: " << leds;
  cr->show_text(sleds.str());

  for(int i=0;i<3;i++)
  {
    int ix,iy;
    ix=48+i*12;
    iy=57;
    cr->move_to(ix,iy);
    if ( leds & (1<<i) )
    {
      cr->set_source_rgb(0.0, 0.8, 0.0);
      cr->arc(ix,iy, 4, 0.0, 2 * M_PI);
      cr->fill();
    } else {
      cr->set_source_rgb(0.4, 0.4, 0.4);
      cr->arc(ix,iy, 4, 0.0, 2 * M_PI);
      cr->fill();
    }
  }

  draw_map(cr);
  draw_car(cr);
  draw_sensordata(cr);

  return true;
}


void MyArea::draw_sensordata(const Cairo::RefPtr<Cairo::Context>& cr){
  cr->set_line_width(2.0);
  double posx = 200.0;
  double posy = 10.0;
  double height = 150.0;
  double spacing = 3;
  cr->set_source_rgba(0.4, 0.0, 0.0,0.4);
  cr->rectangle(posx, posy, spacing * (double)(mySensor.angular_resolution), height);
  cr->stroke();
  cr->rectangle(posx, posy, spacing * (double)(mySensor.angular_resolution), height);
  cr->set_source_rgba(0.4, 0.4, 0.0,0.4);
  cr->fill();
  cr->set_source_rgba(0.0, 0.4, 0.0,0.4);
  int i;
  for(i=0;i<mySensor.angular_resolution;i++){
    cr->move_to(posx + ((double)i) * spacing, posy);
    cr->line_to(posx + ((double)i) * spacing, posy + (mySensor.range-mySensor.data[i])*height/mySensor.range );
  }
  cr->stroke();

  cr->set_source_rgb(0.4, 0.0, 0.0);
  Cairo::RefPtr<Cairo::ToyFontFace> font =
    Cairo::ToyFontFace::create("Bitstream Charter",
                               Cairo::FONT_SLANT_NORMAL,
                               Cairo::FONT_WEIGHT_NORMAL);
  cr->set_font_face(font);
  cr->set_font_size(8.0);

  cr->move_to(posx+1,posy+9);
  std::stringstream sspeed;
  sspeed << "LASERSCANNER";
  cr->show_text(sspeed.str());
  
}

void MyArea::draw_map(const Cairo::RefPtr<Cairo::Context>& cr){
  int i;
  cr->set_line_width(2.0);
  cr->set_source_rgb(0.0, 0.4, 0.0);
  cr->stroke();
  for(i=0;i<myMap.ocount;i++){
    vect2 obs = myCamera.transform(myMap.obstacles[i].pos);
    //cr->move_to(obs.x, obs.y);
    cr->arc(obs.x, obs.y, myCamera.scale(myMap.obstacles[i].r), 0.0, 2 * M_PI);
    cr->stroke();
    
  }

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
  
  //draw sensor:

  cr->set_source_rgb(0.6, 0.0, 0.0);
  vect2 s1 = front + myCar.direction.get_rotated(mySensor.angular) * myCamera.scale(mySensor.range);
  vect2 s2 = front + myCar.direction.get_rotated(-mySensor.angular) * myCamera.scale(mySensor.range);
  cr->move_to(front.x, front.y);
  cr->line_to(s1.x, s1.y);
  cr->move_to(front.x, front.y);
  cr->line_to(s2.x, s2.y);


  //vect2 M = myCamera.transform(myCar.get_center_of_rotation());

  //cr->move_to(back.x, back.y);
  //cr->line_to(M.x, M.y);

  cr->stroke();
  
}
