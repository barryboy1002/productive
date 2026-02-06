#include "tasklayout.h"
#include <iostream>


TodoWindow::TodoWindow():
	OverallCont(Gtk::Orientation::VERTICAL),
	TaskHolder(Gtk::Orientation::VERTICAL){
	
	//setting the application properties
	set_size_request(1000,800);
	set_title("productive");
	set_child(OverallCont);

	//setting the input bar properties
	TaskInput.set_max_length(50);
	
	TaskInput.set_size_request(600,60);
	TaskInput.set_text("Enter your Task now");
	TaskInput.set_icon_from_icon_name("list-add",Gtk::Entry::IconPosition::SECONDARY);
	TaskInput.signal_icon_press().connect(sigc::mem_fun(*this,&TodoWindow::save_task));
	
	TaskInput.set_icon_from_icon_name("edit-find",Gtk::Entry::IconPosition::PRIMARY);
	TaskInput.signal_icon_press().connect(sigc::mem_fun(*this,&TodoWindow::search_for_task));
	TaskHolder.set_vexpand(true);

	//setting up the listview for tasik
	m_scroll.set_child(list_cont);

  	// Only show the scrollbars when they are necessary:
  	m_scroll.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
  	m_scroll.set_expand();
	list_cont.set_selection_mode(Gtk::SelectionMode::SINGLE);
	list_cont.set_size_request(730, -1);
	list_cont.set_halign(Gtk::Align::CENTER);
	list_cont.set_margin_top(20);
	list_cont.set_margin_bottom(20);
	//setting up style
	
	// 1. Create the CSS Provider
	auto css_provider = Gtk::CssProvider::create();
	css_provider->load_from_file(style_file);		// 2. Add it to the default display
	Gtk::StyleContext::add_provider_for_display(
		Gdk::Display::get_default(), 
		css_provider, 
		GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
	);

	TaskHolder.append(m_scroll);
	OverallCont.append(TaskHolder);
	OverallCont.append(TaskInput);
	}

TodoWindow::~TodoWindow(){
}

void TodoWindow::save_task(Gtk::Entry::IconPosition icon_pos){
	if(icon_pos == Gtk::Entry::IconPosition::SECONDARY){
				auto row = Gtk::make_managed<Gtk::ListBoxRow>();
            	auto hbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 30);
            	auto label = Gtk::make_managed<Gtk::Label>("Task #");
            	auto check = Gtk::make_managed<Gtk::CheckButton>();
		label->set_expand(true);
        	label->set_halign(Gtk::Align::START);

        	hbox->append(*label);
        	hbox->append(*check);
        	row->set_child(*hbox);
		row->set_overflow(Gtk::Overflow::HIDDEN);
            
        	list_cont.append(*row);
	}
} 
void TodoWindow::search_for_task(Gtk::Entry::IconPosition icon_pos){

	if(icon_pos == Gtk::Entry::IconPosition::PRIMARY){
		std::cout << "searching" << std::endl;
	}
}
