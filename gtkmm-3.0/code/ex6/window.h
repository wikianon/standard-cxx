#ifndef MY_WINDOW_H
#define MY_WINDOW_H
#include <gtkmm.h>

class MyWindow : public Gtk::Window {
    Glib::RefPtr<Gtk::Application> & app_;
    Glib::RefPtr<Gtk::Builder> & builder_;
    Glib::RefPtr<Gio::SimpleActionGroup> actionGroup_;
    Gtk::Box    * box_;
    Gtk::Image  * image_;
    Gtk::Button * button1_, * button2_;
    Gtk::Label  * label_;
    Gtk::Entry  * valueField_;

    static std::vector<std::string> imageNames_;
    static int index_;

    void buildMenu();
    void buildUI();
    
  protected:
    // signal handlers
    void sub_action1_1(); // pick better names for actual menus!
    void sub_action1_2();
    void sub_action1_quit();
    void sub_action2_1();
    void sub_action2_2();
    void show_image_1();
    void show_image_2();
    void show_image_3();
    void b1_clicked();
    void b2_clicked();
    bool on_key_press_event( GdkEventKey* ) override;

  public:
    MyWindow( Glib::RefPtr<Gtk::Application> &, Glib::RefPtr<Gtk::Builder> & );
    ~MyWindow();
};

#endif
