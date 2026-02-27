#include <iostream>
#include "taskdialog.h"
#include "../storage/taskstorage.h"


TaskDialog::TaskDialog():
	Gen_cont(Gtk::Orientation::VERTICAL),
	priorities_cont(Gtk::Orientation::HORIZONTAL),
	save_b("submit"){
		set_size_request(600,600);
		set_child(Gen_cont);
		t_desc.set_wrap_mode(Gtk::WrapMode::WORD);
		t_desc.get_style_context()->add_class("task-desc");
		
		scrolled.set_child(t_desc);
		scrolled.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
		scrolled.set_propagate_natural_height(true);
		t_name.add_css_class("task-name");
		t_desc.add_css_class("task-desc");
		save_b.add_css_class("submit-task");
		save_b.signal_clicked().connect(sigc::mem_fun(*this,&TaskDialog::on_save));
		//add css:
		auto css_provider_2 =  Gtk::CssProvider::create();
		css_provider_2->load_from_file(style_file2);
		Gtk::StyleContext::add_provider_for_display(
			Gdk::Display::get_default(), 
			css_provider_2, 
			GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
		);
		prior_area_set();
		
		Gen_cont.append(t_name);
		Gen_cont.append(scrolled);
		Gen_cont.append(priorities_cont);
		Gen_cont.append(save_b);

	}
TaskDialog::~TaskDialog(){

}

void TaskDialog::set_text_input_area(){
		
}

void TaskDialog::set_task_txt(const Glib::ustring& t_text,const Glib::ustring& desc_txt,int levl){
	t_name.set_text(t_text);
	auto desc_buffer = t_desc.get_buffer();
  desc_buffer->set_text(desc_txt);
  p_drop.set_selected(levl);
}

void TaskDialog::prior_area_set(){

	cal_button.set_icon_name("x-office-calendar-symbolic");
	cal_button.get_style_context()->add_class("circular-button");
	cal_button.set_has_frame(true);
	const std::vector<Glib::ustring> strings{"High","Mid","Low"};
	priority = Gtk::StringList::create(strings);
	p_drop.set_model(priority);
	p_drop.set_selected(0);
	
	
	auto spacer  = Gtk::make_managed<Gtk::Box>();
	spacer->set_hexpand(true);	
	priorities_cont.append(cal_button);
	priorities_cont.append(*spacer);
	priorities_cont.append(p_drop);
	priorities_cont.add_css_class("priorities-row");

}

void TaskDialog::on_save() {
    auto desc_buffer = t_desc.get_buffer();
    Glib::ustring desc_text = desc_buffer->get_text();
    Glib::ustring name_text = t_name.get_text();

    Task mytask(name_text, desc_text, static_cast<Priority>(p_drop.get_selected()), "1990-12-21");
    int new_id = mytask.add_to_db();

    if (new_id != -1) {
        m_signal_task_saved.emit(new_id, name_text);
        set_visible(false);
    } else {
        std::cerr << "Save failed, not adding row" << std::endl;
    }
}