#include "window.h"
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

int MyWindow::index_ = 0;
std::vector<std::string> MyWindow::imageNames_ = { 
    "img/cat-animal-cat-portrait-mackerel.jpg",
    "img/pexels-photo-208840.jpeg", 
    "img/cat-face-close-view-115011.jpeg" };

MyWindow::MyWindow( Glib::RefPtr<Gtk::Application> & app, Glib::RefPtr<Gtk::Builder> & builder ) : 
    Gtk::Window::Window(), app_{app}, builder_{builder}, actionGroup_{nullptr}, box_{nullptr},
    image_{ nullptr }, button1_{nullptr}, button2_{nullptr}, label_{nullptr}, valueField_{nullptr}
{
    set_title( "Example 6" );
    set_default_size( 300, 400 );  // Set default size, width and height, in pixels.
    set_border_width( 10 );
    buildUI();
    buildMenu();
    add_events( Gdk::KEY_PRESS_MASK );
    show_all_children();
}

MyWindow::~MyWindow() {
} // MyWindow::~MyWindow

void MyWindow::buildUI() {
    Gtk::Window* window_ = nullptr;
    builder_->get_widget( "window", window_ );
    if ( window_ == nullptr ) {
	g_warning("unable to extract window"); 
	return;
    } // if
    Gtk::Widget * tmpWidget = window_->get_child();
    window_->remove();
    add( *tmpWidget );
    builder_->get_widget( "button1", button1_ );
    builder_->get_widget( "button2", button2_ );
    builder_->get_widget( "image", image_ );
    builder_->get_widget( "entry", valueField_ );
    builder_->get_widget( "label", label_ );
    if ( button1_ == nullptr || button2_ == nullptr || image_ == nullptr ||
	valueField_ == nullptr || label_ == nullptr ) {
	g_warning("unable to extract window sub-components"); 
	return;
    } // if
    image_->set( imageNames_.at( 0 ) ); 
    button1_->signal_clicked().connect( sigc::mem_fun(*this, &MyWindow::b1_clicked) );
    button2_->signal_clicked().connect( sigc::mem_fun(*this, &MyWindow::b2_clicked) );
} // MyWindow::buildUI

void MyWindow::buildMenu() {
    actionGroup_ = Gio::SimpleActionGroup::create();
    Gtk::Window::insert_action_group( "example", actionGroup_ ); 

    // Tie handlers to menu choices.
    actionGroup_->add_action( "sub_act_1.1", sigc::mem_fun( *this, &MyWindow::sub_action1_1 ) );
    actionGroup_->add_action( "sub_act_1.2", sigc::mem_fun( *this, &MyWindow::sub_action1_2 ) );
    actionGroup_->add_action( "quit",        sigc::mem_fun( *this, &MyWindow::sub_action1_quit ) );
    actionGroup_->add_action( "sub_act_2.1", sigc::mem_fun( *this, &MyWindow::sub_action2_1 ) );
    actionGroup_->add_action( "sub_act_2.2", sigc::mem_fun( *this, &MyWindow::sub_action2_2 ) );

    // Tie accelerator keys to actions.
    app_->set_accel_for_action("example.sub_act_1.1", "<Primary>1");
    app_->set_accel_for_action("example.sub_act_1.2", "<Primary>2");
    app_->set_accel_for_action("example.quit", "<Primary>q");
    app_->set_accel_for_action("example.sub_act_2.1", "<Primary>3");
    app_->set_accel_for_action("example.sub_act_2.2", "<Primary>4");

    // Tie handlers to toolbar buttons
    actionGroup_->add_action( "show1", sigc::mem_fun( *this, &MyWindow::show_image_1 ) );
    actionGroup_->add_action( "show2", sigc::mem_fun( *this, &MyWindow::show_image_2 ) );
    actionGroup_->add_action( "show3", sigc::mem_fun( *this, &MyWindow::show_image_3 ) );
} // MyWindow::buildMenu

void MyWindow::sub_action1_1() { cout << "sub action 1.1" << endl; }
void MyWindow::sub_action1_2() { cout << "sub action 1.2" << endl; }
void MyWindow::sub_action1_quit() {
    Gtk::MessageDialog dialog( *this, "Termination Dialog", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
    dialog.set_secondary_text( "Are you *really* sure you want to Quit?" );
    int result = dialog.run();
    switch(result) {
      case( Gtk::RESPONSE_OK ):
	  //window_->hide(); //Closes the main window to stop the app->run().
	  hide();
	  break;
      case( Gtk::RESPONSE_CANCEL ):
	  std::cout << "Cancel clicked." << std::endl;
	  break;
      default:
	  std::cout << "Unexpected button clicked." << std::endl;
	  break;
    } // switch
} // MyWindow::sub_action1_quit()

void MyWindow::sub_action2_1() { cout << "sub action 2.1" << endl; }
void MyWindow::sub_action2_2() { cout << "sub action 2.2" << endl; }
void MyWindow::show_image_1() { 
    cout << "show image 1" << endl;
    image_->set( imageNames_.at( 0 ) ); 
}
void MyWindow::show_image_2() { 
    cout << "show image 2" << endl;
    image_->set( imageNames_.at( 1 ) ); 
}
void MyWindow::show_image_3() { 
    cout << "show image 3" << endl;
    image_->set( imageNames_.at( 2 ) ); 
}

void MyWindow::b1_clicked() {
    index_++;
    index_ %= imageNames_.size();
    image_->set( imageNames_.at( index_ ) );
}

void MyWindow::b2_clicked() {
    string s = valueField_->get_text();
    cout << "old text = " << s << endl;
    valueField_->set_text( "yyy" );
}

bool MyWindow::on_key_press_event( GdkEventKey* keyEvent ) {
    static int numTimesReturnPressed = 0;
    if ( keyEvent->keyval == GDK_KEY_Return ) {
	numTimesReturnPressed++;
	string s = valueField_->get_text();
	cout << "User entered: " << s << endl;
	label_->set_text( s );
	valueField_->set_text( std::to_string( numTimesReturnPressed ) );
	return true;
    } // if

    // return true to indicate handled; otherwise call the base class
    return Gtk::Window::on_key_press_event( keyEvent );
} // MyWindow::on_key_press_event
