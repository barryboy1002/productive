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
		void on_task_saved(int task_id, Glib::ustring task_name);
		void on_task_updated(int task_id, Glib::ustring new_name);
	protected:
		//functions
		void show_task(Glib::ustring task_text, int task_id);
		void load_startup_data();
		//signal handlers
		void save_task(Gtk::Entry::IconPosition icon_pos);
		void search_for_task(Gtk::Entry::IconPosition icon_pos);
		void edit_task(Gtk::ListBoxRow* t_row);
    bool check_alarms();
    void send_notification(const std::string& task_name);
		
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
