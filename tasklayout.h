#ifndef GTKMM_TODO_LAYOUT
#define GTKMM_TODO_LAYOUT

#include <gtkmm.h>
#include <giomm.h>
#include "taskdialog.h"

class TodoWindow: public Gtk::Window{
	public:
		TodoWindow();
		virtual ~TodoWindow();
	protected:
		//functions
		void show_task();
		//signal_handlers
		void save_task(Gtk::Entry::IconPosition icon_pos);
		void search_for_task(Gtk::Entry::IconPosition icon_pos);
		void edit_task();
		//widgets for the main window layout
		Gtk::Box OverallCont;
		Gtk::Box TaskHolder;
		Gtk::Entry TaskInput;

		//widgets for tasks layout.
		Gtk::ScrolledWindow m_scroll;
		Gtk::ListBox list_cont;

		//widgets for the pop up 
		TaskDialog Dialog;

		//external files
		Glib::RefPtr<Gio::File> style_file = Gio::File::create_for_path("styles.css");
};

#endif
