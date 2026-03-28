#include "tasklayout.h"
#include <iostream>
#include "../storage/taskstorage.h"

TodoWindow::TodoWindow():
    m_overall_container(Gtk::Orientation::VERTICAL),
    m_task_holder(Gtk::Orientation::VERTICAL),
    m_dialog()
{
    // Set application properties
    set_size_request(1000, 800);
    set_title("productive");
    set_child(m_overall_container);
    
    setup_task_input();
    setup_task_holder();
    setup_dialog();
    load_startup_data();

    // Setup style
    auto context = m_task_holder.get_style_context();
    context->add_class("task-holder");

    // Create and add CSS provider
    auto css_provider = Gtk::CssProvider::create();
    css_provider->load_from_file(m_style_file);
    Gtk::StyleContext::add_provider_for_display(
        Gdk::Display::get_default(),
        css_provider,
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    get_style_context()->add_class("main-window");

    // Add widgets to main layout
    m_task_holder.append(m_scroll_window);
    m_overall_container.append(m_task_holder);
    m_overall_container.append(m_task_input);

    // Connect signals
    m_dialog.signal_task_saved().connect(
        sigc::mem_fun(*this, &TodoWindow::on_task_saved));
    m_dialog.signal_task_updated().connect(
        sigc::mem_fun(*this, &TodoWindow::on_task_updated));

    // Setup alarm checking timer
    Glib::signal_timeout().connect_seconds(
        sigc::mem_fun(*this, &TodoWindow::check_alarms),
        ALARM_CHECK_INTERVAL_SECONDS);
}

TodoWindow::~TodoWindow() {}

void TodoWindow::setup_task_input() {
    m_task_input.set_max_length(50);
    m_task_input.set_size_request(600, 60);
    m_task_input.set_placeholder_text("Enter your Task now");
    m_task_input.set_icon_from_icon_name("list-add", Gtk::Entry::IconPosition::SECONDARY);
    m_task_input.signal_icon_press().connect(
        sigc::mem_fun(*this, &TodoWindow::save_task));

    m_task_input.set_icon_from_icon_name("edit-find", Gtk::Entry::IconPosition::PRIMARY);
    m_task_input.signal_icon_press().connect(
        sigc::mem_fun(*this, &TodoWindow::search_for_task));

    m_task_input.add_css_class("floating-bar");
    m_task_input.set_halign(Gtk::Align::CENTER);
    m_task_input.set_valign(Gtk::Align::END);
    m_task_input.set_size_request(450, -1);
    m_task_input.set_margin_bottom(20);
}

void TodoWindow::setup_task_holder() {
    m_scroll_window.set_has_frame(false);
    m_scroll_window.set_css_classes({});
    m_scroll_window.set_propagate_natural_width(true);
    m_task_holder.set_vexpand(true);
    m_scroll_window.set_child(m_task_list);
    m_scroll_window.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    m_scroll_window.set_expand();

    m_task_list.set_selection_mode(Gtk::SelectionMode::SINGLE);
    m_task_list.set_size_request(730, -1);
    m_task_list.set_halign(Gtk::Align::CENTER);
    m_task_list.set_margin_top(20);
    m_task_list.set_margin_bottom(20);
    m_task_list.signal_row_activated().connect(
        sigc::mem_fun(*this, &TodoWindow::edit_task), nullptr);
}

void TodoWindow::setup_dialog() {
    m_dialog.set_default_size(250, 100);
    m_dialog.set_transient_for(*this);
    m_dialog.set_modal();
    m_dialog.set_hide_on_close();
}

void TodoWindow::on_task_saved(int task_id, Glib::ustring task_name) {
    show_task(task_name, task_id);
}

void TodoWindow::on_task_updated(int task_id, Glib::ustring new_name) {
    // Find and update the task row by task_id
    auto* row = m_task_list.get_row_at_index(0);
    int index = 0;
    
    while (row != nullptr) {
        if (GPOINTER_TO_INT(row->get_data("task_id")) == task_id) {
            auto* box = dynamic_cast<Gtk::Box*>(row->get_child());
            if (box) {
                auto* label = dynamic_cast<Gtk::Label*>(box->get_first_child());
                if (label) {
                    label->set_text(new_name);
                }
            }
            break;
        }
        row = m_task_list.get_row_at_index(++index);
    }
}

void TodoWindow::save_task(Gtk::Entry::IconPosition icon_pos) {
    if (icon_pos == Gtk::Entry::IconPosition::SECONDARY) {
        m_dialog.set_add_mode();
        m_dialog.set_task_txt(m_task_input.get_text(), "", 0);
        m_dialog.set_visible(true);
    }
}

void TodoWindow::show_task(Glib::ustring task_text, int task_id) {
    auto row     = Gtk::make_managed<Gtk::ListBoxRow>();
    auto hbox    = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 12);
    auto label   = Gtk::make_managed<Gtk::Label>(task_text);
    auto check   = Gtk::make_managed<Gtk::CheckButton>();
    auto del_btn = Gtk::make_managed<Gtk::Button>();

    // Store task_id on the row
    row->set_data("task_id", GINT_TO_POINTER(task_id));

    // Setup delete button
    del_btn->set_icon_name("user-trash-symbolic");
    del_btn->add_css_class("delete-btn");
    del_btn->set_has_frame(false);
    del_btn->set_valign(Gtk::Align::CENTER);

    del_btn->signal_clicked().connect([this, row, task_id]() {
        try {
            pqxx::work tx(conn);
            tx.exec("DELETE FROM taskinfo WHERE tasknum = $1", pqxx::params(task_id));
            tx.commit();
            m_task_list.remove(*row);
        } catch (const std::exception& e) {
            std::cerr << "Delete error: " << e.what() << std::endl;
        }
    });

    // Setup label
    hbox->add_css_class("task-row-box");
    label->add_css_class("task-label");
    label->set_expand(true);
    label->set_halign(Gtk::Align::START);

    // Assemble the row
    hbox->append(*label);
    hbox->append(*check);
    hbox->append(*del_btn);
    row->set_child(*hbox);
    row->set_overflow(Gtk::Overflow::HIDDEN);

    m_task_list.append(*row);
}

void TodoWindow::search_for_task(Gtk::Entry::IconPosition icon_pos) {
    if (icon_pos == Gtk::Entry::IconPosition::PRIMARY) {
        std::cout << "Search functionality not yet implemented" << std::endl;
    }
}

void TodoWindow::edit_task(Gtk::ListBoxRow* task_row) {
    if (!task_row) {
        return;
    }

    int task_id = GPOINTER_TO_INT(task_row->get_data("task_id"));
    if (task_id == -1) {
        std::cerr << "Row has no valid task_id" << std::endl;
        return;
    }

    auto tsk = get_task_info(task_id);
    if (!tsk) {
        std::cerr << "Task not found in database for id: " << task_id << std::endl;
        return;
    }

    m_dialog.set_edit_mode(task_id);
    m_dialog.set_task_txt(tsk->name, tsk->desc, static_cast<int>(tsk->level));
    m_dialog.set_visible(true);
}

void TodoWindow::load_startup_data() {
    auto tasklist = get_taskname_list();
    for (const auto& task : tasklist) {
        show_task(task.task_name, task.task_id);
    }
}

bool TodoWindow::check_alarms() {
    try {
        ensure_connected();
        pqxx::nontransaction tx(conn);
        
        // Find tasks whose duedate is within the next ALARM_CHECK_INTERVAL_SECONDS
        auto result = tx.exec(
            "SELECT tasknum, taskname FROM taskinfo "
            "WHERE duedate BETWEEN NOW() AND NOW() + INTERVAL '" +
            std::to_string(ALARM_CHECK_INTERVAL_SECONDS) + " seconds'");

        for (auto row : result) {
            int id           = row[0].as<int>();
            std::string name = row[1].as<std::string>();
            send_notification(name);
        }
    } catch (const std::exception& e) {
        std::cerr << "Alarm check error: " << e.what() << std::endl;
    }
    return true;  // Keep timer running
}

void TodoWindow::send_notification(const std::string& task_name) {
    auto app = Gio::Application::get_default();
    auto notification = Gio::Notification::create("Task Due");
    notification->set_body(task_name);
    notification->set_priority(Gio::Notification::Priority::URGENT);
    app->send_notification(task_name, notification);
}