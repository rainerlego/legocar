#include "myarea.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>

int main(int argc, char** argv)
{
   Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

   Gtk::Window win;
   win.set_title("DrawingArea");

   MyArea area;
   win.add(area);
   area.show();

   return app->run(win);
}

