#include <gtkmm/window.h>	    // Gtk::Window
#include <gtkmm/frame.h>	    // Gtk::Frame
#include <gtkmm/paned.h>	    // Gtk::Paned

class MyWindow : public Gtk::Window {
	Gtk::Paned paned_;
	Gtk::Frame frame1_, frame2_;
public:
	MyWindow();
	~MyWindow();
};