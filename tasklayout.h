#ifndef GTKMM_TODO_LAYOUT
#define GTKMM_TODO_LAYOUT

#include <gtkmm.h>

class TodoWindow: public Gtk::Window{
	public:
		TodoWindow();
		virtual ~TodoWindow();
	protected:
		//signal_handlers
		void save_task(Gtk::Entry::IconPosition icon_pos);
		void search_for_task(Gtk::Entry::IconPosition icon_pos);
		//widgets
		Gtk::Box OverallCont;
		Gtk::Box TaskHolder;
		Gtk::Entry TaskInput;
};

#endif
