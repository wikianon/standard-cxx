#include <gtkmm.h>
#include <vector>
#include <string>

class MyWindow : public Gtk::Window {
	Gtk::Paned paned_;
	Gtk::Frame frame1_, frame2_;
	Gtk::ButtonBox bbox_;
	Gtk::Box box_;
	Gtk::Label label_;
	Gtk::Button button1_, button2_;
	Gtk::Image image_;
	Gtk::Entry valueField_;
	static std::vector<std::string> imageNames_;
	static int index_;

	bool on_key_press_event( GdkEventKey* ) override;

protected:
	// signal handlers
	void b1_clicked();
	void b2_clicked();

public:
	MyWindow();
	~MyWindow();
};