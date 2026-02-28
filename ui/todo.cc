#include "tasklayout.h"
#include <iostream>
#include "../storage/taskstorage.h"



std::vector<task_state> tasklist = get_taskname_list();

TodoWindow::TodoWindow():
	OverallCont(Gtk::Orientation::VERTICAL),
	TaskHolder(Gtk::Orientation::VERTICAL),
	Dialog(){
	
	//setting the application properties
	set_size_request(1000,800);
	set_title("productive");
	set_child(OverallCont);
	taskInputSet();
	taskHolderSet();
	dialogSet();
	load_startup_data();
	//setting up style
	auto context = TaskHolder.get_style_context();
	context->add_class("task-holder");

	
	// 1. Create the CSS Provider
	auto css_provider = Gtk::CssProvider::create();
	css_provider->load_from_file(style_file);		// 2. Add it to the default display
	Gtk::StyleContext::add_provider_for_display(
		Gdk::Display::get_default(), 
		css_provider, 
		GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
	);
	get_style_context()->add_class("main-window");

	TaskHolder.append(m_scroll);
	OverallCont.append(TaskHolder);
	OverallCont.append(TaskInput);
	Dialog.signal_task_saved().connect(
        sigc::mem_fun(*this, &TodoWindow::on_task_saved)
    );
	Dialog.signal_task_updated().connect(
    sigc::mem_fun(*this, &TodoWindow::on_task_updated)
	);
  Glib::signal_timeout().connect_seconds(
    sigc::mem_fun(*this, &TodoWindow::check_alarms), 30);

	}

TodoWindow::~TodoWindow(){
}

void TodoWindow::taskInputSet(){
	
	//setting the input bar properties
	TaskInput.set_max_length(50);
	
	TaskInput.set_size_request(600,60);
	TaskInput.set_placeholder_text("Enter your Task now");
	TaskInput.set_icon_from_icon_name("list-add",Gtk::Entry::IconPosition::SECONDARY);
	TaskInput.signal_icon_press().connect(sigc::mem_fun(*this,&TodoWindow::save_task));
	
	TaskInput.set_icon_from_icon_name("edit-find",Gtk::Entry::IconPosition::PRIMARY);

	TaskInput.signal_icon_press().connect(sigc::mem_fun(*this,&TodoWindow::search_for_task));
	TaskInput.add_css_class("floating-bar");
	
	// 2. Setup the Search Bar Container (e.g., a Gtk::Box)
	TaskInput.add_css_class("floating-bar");
	TaskInput.set_halign(Gtk::Align::CENTER); // Centers it horizontally
	TaskInput.set_valign(Gtk::Align::END);    // Pins it to the bottom
	TaskInput.set_size_request(450, -1);      // Matches the list width
	TaskInput.set_margin_bottom(20);

}

void TodoWindow::taskHolderSet() {
    m_scroll.set_has_frame(false);
    m_scroll.set_css_classes({});  // strip any theme classes
    m_scroll.set_propagate_natural_width(true);
    TaskHolder.set_vexpand(true);
    m_scroll.set_child(list_cont);
    m_scroll.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    m_scroll.set_expand();
    list_cont.set_selection_mode(Gtk::SelectionMode::SINGLE);
    list_cont.set_size_request(730, -1);
    list_cont.set_halign(Gtk::Align::CENTER);
    list_cont.set_margin_top(20);
    list_cont.set_margin_bottom(20);
    list_cont.signal_row_activated().connect(sigc::mem_fun(*this, &TodoWindow::edit_task), NULL);
}


void TodoWindow::dialogSet(){
	Dialog.set_default_size(250, 100);
  	Dialog.set_transient_for(*this);
 	Dialog.set_modal();
	Dialog.set_hide_on_close();}

void TodoWindow::on_task_saved(int task_id, Glib::ustring task_name) {
    show_task(task_name, task_id);
    // also keep tasklist in sync so edit_task() works on new rows
    tasklist.push_back(task_state{task_id, std::string(task_name)});
}

// new handler — finds the row by its stored task_id and updates its label
void TodoWindow::on_task_updated(int task_id, Glib::ustring new_name) {
    // walk the listbox rows and find the one with matching task_id
    auto* row = list_cont.get_row_at_index(0);
    int i = 0;
    while (row != nullptr) {
        if (GPOINTER_TO_INT(row->get_data("task_id")) == task_id) {
            // navigate to the label: row -> hbox -> first child
            auto* box   = dynamic_cast<Gtk::Box*>(row->get_child());
            if (box) {
                auto* label = dynamic_cast<Gtk::Label*>(box->get_first_child());
                if (label) label->set_text(new_name);
            }
            break;
        }
        row = list_cont.get_row_at_index(++i);
    }
}

void TodoWindow::save_task(Gtk::Entry::IconPosition icon_pos){
	if(icon_pos == Gtk::Entry::IconPosition::SECONDARY){
		//setting up the dialogs additional properties
		Dialog.set_add_mode();
		Dialog.set_task_txt(TaskInput.get_text(),"",0);
		Dialog.set_visible(true);

	}
}

void TodoWindow::show_task(Glib::ustring task_text, int task_id) {
    auto row     = Gtk::make_managed<Gtk::ListBoxRow>();
    auto hbox    = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 12);
    auto label   = Gtk::make_managed<Gtk::Label>(task_text);
    auto check   = Gtk::make_managed<Gtk::CheckButton>();
    auto del_btn = Gtk::make_managed<Gtk::Button>();

    // store task_id directly on the row using set_data
    row->set_data("task_id", GINT_TO_POINTER(task_id));

    del_btn->set_icon_name("user-trash-symbolic");
    del_btn->add_css_class("delete-btn");
    del_btn->set_has_frame(false);
    del_btn->set_valign(Gtk::Align::CENTER);

    del_btn->signal_clicked().connect([this, row, task_id]() {
        try {
            pqxx::work tx(conn);
            tx.exec("DELETE FROM taskinfo WHERE tasknum = $1", pqxx::params(task_id));
            tx.commit();
            list_cont.remove(*row);
        } catch (const std::exception &e) {
            std::cerr << "Delete error: " << e.what() << std::endl;
        }
    });

    hbox->add_css_class("task-row-box");
    label->add_css_class("task-label");
    label->set_expand(true);
    label->set_halign(Gtk::Align::START);

    hbox->append(*label);
    hbox->append(*check);
    hbox->append(*del_btn);
    row->set_child(*hbox);
    row->set_overflow(Gtk::Overflow::HIDDEN);

    list_cont.append(*row);
}

void TodoWindow::search_for_task(Gtk::Entry::IconPosition icon_pos){

	if(icon_pos == Gtk::Entry::IconPosition::PRIMARY){
		std::cout << "searching" << std::endl;
	}
}



void TodoWindow::edit_task(Gtk::ListBoxRow* t_row) {
    if (!t_row) return;

    // read task_id stored on the row at creation time
    int task_id = GPOINTER_TO_INT(t_row->get_data("task_id"));
    if (task_id == -1) {
        std::cerr << "Row has no valid task_id" << std::endl;
        return;
    }

    auto tsk = get_task_info(task_id);
    if (!tsk) {
        std::cerr << "Task not found in db for id: " << task_id << std::endl;
        return;
    }

	Dialog.set_edit_mode(task_id);
    Dialog.set_task_txt(tsk->name, tsk->desc, static_cast<int>(tsk->levl));
    Dialog.set_visible(true);
}


void TodoWindow::load_startup_data() {
    for (auto const &t : tasklist) {
        show_task(t.task_name, t.task_id);
    }
}
// todo.cc
bool TodoWindow::check_alarms() {
    try {
        ensure_connected();
        pqxx::nontransaction tx(conn);
        // find tasks whose duedate is within the next 30 seconds
        auto result = tx.exec(
            "SELECT tasknum, taskname FROM taskinfo "
            "WHERE duedate BETWEEN NOW() AND NOW() + INTERVAL '30 seconds'");

        for (auto row : result) {
            int id              = row[0].as<int>();
            std::string name    = row[1].as<std::string>();
            send_notification(name);
        }
    } catch (const std::exception &e) {
        std::cerr << "Alarm check error: " << e.what() << std::endl;
    }
    return true;  // returning true keeps the timer running
}

void TodoWindow::send_notification(const std::string& task_name) {
    auto app = Gio::Application::get_default();
    auto notification = Gio::Notification::create("Task Due");
    notification->set_body(task_name);
    notification->set_priority(Gio::Notification::Priority::URGENT);
    app->send_notification(task_name, notification);
}
