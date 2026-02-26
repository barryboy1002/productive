#ifndef TASK_DIALOG_H
#define TASK_DIALOG_H

#include <gtkmm.h>


class TaskDialog :public Gtk::Window{
	public:
		TaskDialog();
		~TaskDialog() override;
		void set_task_txt(const Glib::ustring& t_text,const Glib::ustring& t_desc,int levl);

	protected:
		//signal handlers
		void on_save();
		void on_cancel();
		//for setting up the widgets to decluster the conctructor definition
		void prior_area_set();
		void set_text_input_area();
		//widgets
		Gtk::ScrolledWindow scrolled;
		Gtk::Box Gen_cont;
		Gtk::Entry t_name;
		Gtk::TextView t_desc;
		Gtk::Box priorities_cont;
		Gtk::Button cal_button;
		Gtk::DropDown p_drop;
		
		Glib::RefPtr<Gtk::StringList> priority;
		Glib::RefPtr<Gtk::TextBuffer> m_txtbuffer;

		Gtk::Button save_b;
		
		Glib::RefPtr<Gio::File> style_file2 = Gio::File::create_for_path("styles2.css");

};
#endif
