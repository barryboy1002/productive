#include "tasklayout.h"
#include <iostream>


TodoWindow::TodoWindow():
	OverallCont(Gtk::Orientation::VERTICAL),
	TaskHolder(Gtk::Orientation::VERTICAL),
	Dialog(){
	
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
	TaskInput.add_css_class("floating-bar");
	m_scroll.set_has_frame(false);
	m_scroll.set_propagate_natural_width(true);

	// 2. Setup the Search Bar Container (e.g., a Gtk::Box)
	TaskInput.add_css_class("floating-bar");
	TaskInput.set_halign(Gtk::Align::CENTER); // Centers it horizontally
	TaskInput.set_valign(Gtk::Align::END);    // Pins it to the bottom
	TaskInput.set_size_request(450, -1);      // Matches the list width
	TaskInput.set_margin_bottom(20);
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
		show_task();
		Dialog.set_visible(true);
	}
}

void TodoWindow::show_task(){
		Glib::ustring task_text = TaskInput.get_text();
		auto row = Gtk::make_managed<Gtk::ListBoxRow>();
            	auto hbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 30);
            	auto label = Gtk::make_managed<Gtk::Label>(task_text);
            	auto check = Gtk::make_managed<Gtk::CheckButton>();
		label->set_expand(true);
        	label->set_halign(Gtk::Align::START);

        	hbox->append(*label);
        	hbox->append(*check);
        	row->set_child(*hbox);
		row->set_overflow(Gtk::Overflow::HIDDEN);
            
        	list_cont.append(*row);
}

void TodoWindow::search_for_task(Gtk::Entry::IconPosition icon_pos){

	if(icon_pos == Gtk::Entry::IconPosition::PRIMARY){
		std::cout << "searching" << std::endl;
	}
}
