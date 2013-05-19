#include "myarea.h"
#include <cairomm/context.h>
#include "vectors.h"
MyArea::MyArea()
{
}

MyArea::~MyArea()
{
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

  cr->set_line_width(2.0);

   
  // draw red lines out from the center of the window
  cr->set_source_rgb(0.8, 0.0, 0.0);
  
  cr->move_to(0, 0);
  cr->line_to(xc, yc);
  cr->line_to(0, height);
  cr->move_to(xc, yc);
  cr->line_to(width, yc);
  cr->stroke();
  draw_car(cr);

  return true;
}

void MyArea::draw_car(const Cairo::RefPtr<Cairo::Context>& cr){
  vect2 back = myCamera.transform(myCar.get_pos_back());
  vect2 front = myCamera.transform(myCar.get_pos_front());
  cr->set_line_width(4.0);
  cr->set_source_rgb(0.0, 0.0, 0.6);
  cr->move_to(back.x, back.y);
  cr->line_to(front.x, front.y);
  cr->stroke();
  
}
