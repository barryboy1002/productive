#include "tasklayout.h"
#include <gtkmm/application.h>


int main(int argc,char *argv[] ){
	auto app = Gtk::Application::create("com.gmail.odorobarry");
	//show the window and return when it is closed.
	return app->make_window_and_run<TodoWindow>(argc,argv);
}
