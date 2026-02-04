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

	OverallCont.append(TaskHolder);
	OverallCont.append(TaskInput);
	}

TodoWindow::~TodoWindow(){
}

void TodoWindow::save_task(Gtk::Entry::IconPosition icon_pos){
	if(icon_pos == Gtk::Entry::IconPosition::SECONDARY){
		std::cout << "saved" << std::endl;
	}
} 
void TodoWindow::search_for_task(Gtk::Entry::IconPosition icon_pos){

	if(icon_pos == Gtk::Entry::IconPosition::PRIMARY){
		std::cout << "searching" << std::endl;
	}
}
