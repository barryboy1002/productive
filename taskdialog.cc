#include <iostream>
#include "taskdialog.h"


TaskDialog::TaskDialog():
	Gen_cont(Gtk::Orientation::VERTICAL),
	save_b("submit"){
		set_child(Gen_cont);
		t_name.set_placeholder_text("enter the task name");
		t_desc.set_placeholder_text("description");

		t_name.add_css_class("task-name");
		t_desc.add_css_class("task-desc");
		save_b.add_css_class("submit-task");
		//add css
		auto css_provider_2 =  Gtk::CssProvider::create();
		css_provider_2->load_from_file(style_file2);
		Gtk::StyleContext::add_provider_for_display(
			Gdk::Display::get_default(), 
			css_provider_2, 
			GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
		);


		Gen_cont.append(t_name);
		Gen_cont.append(t_desc);
		Gen_cont.append(save_b);

	}
TaskDialog::~TaskDialog(){
}
