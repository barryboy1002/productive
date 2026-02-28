#include "taskdialog.h"
#include "../storage/taskstorage.h"
#include <iostream>
#include <sstream>
#include <iomanip>

TaskDialog::TaskDialog():
    Gen_cont(Gtk::Orientation::VERTICAL),
    priorities_cont(Gtk::Orientation::HORIZONTAL),
    dt_popover_cont(Gtk::Orientation::VERTICAL, 8),
    time_row(Gtk::Orientation::HORIZONTAL, 4),
    colon_label(":"),
    confirm_dt_btn("Set"),
    save_b("submit")
{
    set_size_request(600, 600);
    set_child(Gen_cont);

    t_desc.set_wrap_mode(Gtk::WrapMode::WORD);
    scrolled.set_child(t_desc);
    scrolled.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
    scrolled.set_propagate_natural_height(true);

    t_name.add_css_class("task-name");
    t_desc.add_css_class("task-desc");
    save_b.add_css_class("submit-task");
    save_b.signal_clicked().connect(sigc::mem_fun(*this, &TaskDialog::on_save));

    auto css_provider_2 = Gtk::CssProvider::create();
    css_provider_2->load_from_file(style_file2);
    Gtk::StyleContext::add_provider_for_display(
        Gdk::Display::get_default(),
        css_provider_2,
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    prior_area_set();

    Gen_cont.append(t_name);
    Gen_cont.append(scrolled);
    Gen_cont.append(priorities_cont);
    Gen_cont.append(save_b);
}

TaskDialog::~TaskDialog() {}

void TaskDialog::set_add_mode() {
    m_mode = DialogMode::Add;
    m_editing_id = -1;
    m_selected_datetime = "";
    save_b.set_label("submit");
}

void TaskDialog::set_edit_mode(int task_id) {
    m_mode = DialogMode::Edit;
    m_editing_id = task_id;
    save_b.set_label("update");
}

void TaskDialog::set_task_txt(const Glib::ustring& t_text, const Glib::ustring& desc_txt,
                               int levl, const std::string& duedate) {
    t_name.set_text(t_text);
    t_desc.get_buffer()->set_text(desc_txt);
    p_drop.set_selected(levl);
    m_selected_datetime = duedate;

    // if a duedate exists, pre-populate the calendar and spinners
    if (!duedate.empty() && duedate.size() >= 16) {
        try {
            int year  = std::stoi(duedate.substr(0, 4));
            int month = std::stoi(duedate.substr(5, 2));
            int day   = std::stoi(duedate.substr(8, 2));
            int hour  = std::stoi(duedate.substr(11, 2));
            int min   = std::stoi(duedate.substr(14, 2));

            Glib::DateTime dt = Glib::DateTime::create_local(year, month, day, hour, min, 0);
            m_calendar.select_day(dt);
            hour_spin.set_value(hour);
            min_spin.set_value(min);
            // update button label to show set time
            cal_button.set_label(duedate.substr(0, 16));
        } catch (...) {
            cal_button.set_icon_name("alarm-symbolic");
        }
    } else {
        cal_button.set_icon_name("alarm-symbolic");
    }
}

void TaskDialog::prior_area_set() {
    // clock button
    cal_button.set_icon_name("alarm-symbolic");
    cal_button.add_css_class("circular-button");
    cal_button.set_has_frame(true);
    cal_button.signal_clicked().connect(sigc::mem_fun(*this, &TaskDialog::on_clock_clicked));

    // hour spinner 0-23
    auto hour_adj = Gtk::Adjustment::create(0, 0, 23, 1);
    hour_spin.set_adjustment(hour_adj);
    hour_spin.set_wrap(true);
    hour_spin.set_width_chars(2);
    hour_spin.set_numeric(true);

    // minute spinner 0-59
    auto min_adj = Gtk::Adjustment::create(0, 0, 59, 1);
    min_spin.set_adjustment(min_adj);
    min_spin.set_wrap(true);
    min_spin.set_width_chars(2);
    min_spin.set_numeric(true);

    // time row: HH : MM
    time_row.set_halign(Gtk::Align::CENTER);
    time_row.append(hour_spin);
    time_row.append(colon_label);
    time_row.append(min_spin);

    confirm_dt_btn.add_css_class("submit-task");
    confirm_dt_btn.signal_clicked().connect([this]() {
        m_selected_datetime = get_selected_datetime();
        // update button to show the chosen time as label
        cal_button.set_label(m_selected_datetime.substr(0, 16));
        dt_popover.set_visible(false);
    });

    // popover contents
    dt_popover_cont.set_margin_top(8);
    dt_popover_cont.set_margin_bottom(8);
    dt_popover_cont.set_margin_start(8);
    dt_popover_cont.set_margin_end(8);
    dt_popover_cont.append(m_calendar);
    dt_popover_cont.append(time_row);
    dt_popover_cont.append(confirm_dt_btn);

    dt_popover.set_child(dt_popover_cont);
    dt_popover.set_parent(cal_button);  // anchor popover to the clock button

    // layout
    auto spacer = Gtk::make_managed<Gtk::Box>();
    spacer->set_hexpand(true);
    priorities_cont.add_css_class("priorities-row");
    priorities_cont.append(cal_button);
    priorities_cont.append(*spacer);
    priorities_cont.append(p_drop);

    // priority dropdown
    const std::vector<Glib::ustring> strings{"High", "Mid", "Low"};
    priority = Gtk::StringList::create(strings);
    p_drop.set_model(priority);
    p_drop.set_selected(0);
}

void TaskDialog::on_clock_clicked() {
    dt_popover.set_visible(true);
}

std::string TaskDialog::get_selected_datetime() {
    Glib::DateTime selected = m_calendar.get_date();
    int hour = static_cast<int>(hour_spin.get_value());
    int min  = static_cast<int>(min_spin.get_value());

    std::ostringstream oss;
    oss.imbue(std::locale::classic());  // force C locale, no thousand separators
    oss << std::setfill('0')
        << selected.get_year()  << "-"
        << std::setw(2) << selected.get_month() << "-"
        << std::setw(2) << selected.get_day_of_month() << " "
        << std::setw(2) << hour << ":"
        << std::setw(2) << min  << ":00";
    return oss.str();
}

void TaskDialog::on_save() {
    Glib::ustring name_text = t_name.get_text();
    Glib::ustring desc_text = t_desc.get_buffer()->get_text();
    Priority priority_val   = static_cast<Priority>(p_drop.get_selected());
    std::string dt          = m_selected_datetime.empty() ? "1990-01-01 00:00:00" : m_selected_datetime;

    if (m_mode == DialogMode::Add) {
        Task mytask(std::string(name_text), std::string(desc_text), priority_val, dt);
        int new_id = mytask.add_to_db();
        if (new_id != -1) {
            m_signal_task_saved.emit(new_id, name_text);
            set_visible(false);
        }
    } else {
        try {
            pqxx::work tx(conn);
            tx.exec(
                "UPDATE taskinfo SET taskname=$1, taskdesc=$2, priority=$3, duedate=$4 "
                "WHERE tasknum=$5",
                pqxx::params(std::string(name_text), std::string(desc_text), priority_val, dt, m_editing_id));
            tx.commit();
            m_signal_task_updated.emit(m_editing_id, name_text);
            set_visible(false);
        } catch (const std::exception &e) {
            std::cerr << "Update error: " << e.what() << std::endl;
        }
    }
}
