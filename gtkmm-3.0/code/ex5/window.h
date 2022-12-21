#include <gtkmm.h>
#include <vector>
#include <string>

class MyWindow : public Gtk::Window {
    const Glib::RefPtr<Gtk::Application>& app_;
    Gtk::Box mainBox_;
    Gtk::Paned paned_;
    Gtk::Frame frame1_, frame2_;
    Gtk::ToolItem item1_, item2_, item3_;
    Gtk::ButtonBox bbox_;
    Gtk::Box box_;
    Gtk::Label label_;
    Gtk::Button button1_, button2_;
    Gtk::Image image_;
    Gtk::Entry valueField_;

    // smart pointers
    Glib::RefPtr<Gtk::Builder> builder_;
    Glib::RefPtr<Gio::SimpleActionGroup> actionGroup_;

    static std::vector<std::string> imageNames_;
    static int index_;

    // signal handler
    bool on_key_press_event( GdkEventKey* ) override;

    void setUpMenu();
    void structureGUI();

  protected:
    // signal handlers
    void b1_clicked();
    void b2_clicked();
    void sub_action1_1(); // pick better names for actual menus!
    void sub_action1_2();
    void sub_action1_quit();
    void sub_action2_1();
    void sub_action2_2();
    void show_image_1();
    void show_image_2();
    void show_image_3();

  public:
    MyWindow( const Glib::RefPtr<Gtk::Application> & app );
    virtual ~MyWindow();
};
