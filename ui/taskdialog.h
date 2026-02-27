#ifndef TASK_DIALOG_H
#define TASK_DIALOG_H
#include <gtkmm.h>

enum class DialogMode { Add, Edit };

class TaskDialog : public Gtk::Window {
public:
    TaskDialog();
    ~TaskDialog() override;
    void set_task_txt(const Glib::ustring& t_text, const Glib::ustring& t_desc, int levl);
    void set_edit_mode(int task_id);  // call this when opening for editing
    void set_add_mode();              // call this when opening for a new task

    sigc::signal<void(int, Glib::ustring)> signal_task_saved() { return m_signal_task_saved; }
    sigc::signal<void(int, Glib::ustring)> signal_task_updated() { return m_signal_task_updated; }

protected:
    void on_save();
    void prior_area_set();
    void set_text_input_area();

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

    DialogMode m_mode = DialogMode::Add;
    int m_editing_id  = -1;  // valid only in Edit mode

    sigc::signal<void(int, Glib::ustring)> m_signal_task_saved;
    sigc::signal<void(int, Glib::ustring)> m_signal_task_updated;
};
#endif