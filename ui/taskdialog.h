#ifndef TASK_DIALOG_H
#define TASK_DIALOG_H
#include <gtkmm.h>
#include <string>

enum class DialogMode { Add, Edit };

class TaskDialog : public Gtk::Window {
public:
    TaskDialog();
    ~TaskDialog() override;
    void set_task_txt(const Glib::ustring& t_text, const Glib::ustring& t_desc,
                      int levl, const std::string& duedate = "");
    void set_edit_mode(int task_id);
    void set_add_mode();

    sigc::signal<void(int, Glib::ustring)> signal_task_saved()   { return m_signal_task_saved; }
    sigc::signal<void(int, Glib::ustring)> signal_task_updated() { return m_signal_task_updated; }

protected:
    void on_save();
    void prior_area_set();
    void on_clock_clicked();         // opens the datetime popover
    std::string get_selected_datetime();  // returns "YYYY-MM-DD HH:MM:SS"

    Gtk::ScrolledWindow scrolled;
    Gtk::Box Gen_cont;
    Gtk::Entry t_name;
    Gtk::TextView t_desc;
    Gtk::Box priorities_cont;

    // clock button + popover
    Gtk::Button cal_button;
    Gtk::Popover dt_popover;
    Gtk::Box dt_popover_cont;
    Gtk::Calendar m_calendar;
    Gtk::Box time_row;
    Gtk::SpinButton hour_spin;
    Gtk::SpinButton min_spin;
    Gtk::Label colon_label;
    Gtk::Button confirm_dt_btn;

    Gtk::DropDown p_drop;
    Glib::RefPtr<Gtk::StringList> priority;
    Glib::RefPtr<Gtk::TextBuffer> m_txtbuffer;
    Gtk::Button save_b;
    Glib::RefPtr<Gio::File> style_file2 = Gio::File::create_for_path("styles2.css");

    DialogMode m_mode      = DialogMode::Add;
    int        m_editing_id = -1;
    std::string m_selected_datetime = "";  // set when user confirms datetime

    sigc::signal<void(int, Glib::ustring)> m_signal_task_saved;
    sigc::signal<void(int, Glib::ustring)> m_signal_task_updated;
};
#endif
