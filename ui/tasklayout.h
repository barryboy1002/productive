#ifndef GTKMM_TODO_LAYOUT
#define GTKMM_TODO_LAYOUT

#include <gtkmm.h>
#include <giomm.h>
#include "taskdialog.h"

class TodoWindow: public Gtk::Window{
	public:
		TodoWindow();
		virtual ~TodoWindow();
		//to declutter the constructor  i am going to use functions called setups
		void taskInputSet();
		void dialogSet();
		void taskHolderSet();
	protected:
		//functions
		void show_task(Glib::ustring task_text);
		//signal_handlers
		void load_startup_data();
		void save_task(Gtk::Entry::IconPosition icon_pos);
		void search_for_task(Gtk::Entry::IconPosition icon_pos);
		void edit_task(Gtk::ListBoxRow* t_row);
		
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
