#ifndef TASK_DIALOG_H
#define TASK_DIALOG_H

#include <gtkmm.h>


class TaskDialog :public Gtk::Window{
	public:
		TaskDialog();
		~TaskDialog() override;
		void set_task_txt(const Glib::ustring& text);

	protected:
		//signal handlers
		void on_save();
		void on_cancel();
		
		//widgets
		Gtk::ScrolledWindow scrolled;
		Gtk::Box Gen_cont;
		Gtk::Entry t_name;
		Gtk::TextView t_desc;
		Gtk::Button save_b;
		
		Glib::RefPtr<Gio::File> style_file2 = Gio::File::create_for_path("styles2.css");

};
#endif
