#include "window.h"
#include <iostream>
using std::cout;
using std::endl;
using std::string;

int MyWindow::index_ = 0;
std::vector<std::string> MyWindow::imageNames_ = { 
	"img/cat-animal-cat-portrait-mackerel.jpg",
	"img/pexels-photo-208840.jpeg", 
	"img/cat-face-close-view-115011.jpeg" };

MyWindow::MyWindow() : 
	paned_{Gtk::Orientation::ORIENTATION_VERTICAL}, 
	label_{"Enter text: "},
	image_{ MyWindow::imageNames_.at( index_ ) }
{
	set_title( "Example 4" );      // Sets the window title.
	set_default_size( 500, 300 );  // Set default size, width and height, in pixels.
	set_border_width( 10 );

	button1_.set_image( image_ );
	button2_.set_label( "something" );
	bbox_.add( button1_ );
	bbox_.add( button2_ );
	valueField_.set_text( "xxx" );
	box_.add( label_ );
	box_.add( valueField_ );

	frame1_.add( bbox_ );
	frame2_.add( box_ );

	add( paned_ );
	paned_.add1( frame1_ );
	paned_.add2( frame2_ );
	frame1_.set_label( "Gtk::Frame 1 Widget" ); // set the frames label
	frame2_.set_label( "Gtk::Frame 2 Widget" ); 

	// xalign (0.0 = left, 1.0 = right, defaults to 0.0) & 
	// yalign (0.0 = under, 1.0 = over, default = 0.5; won't paint gap if exactly 0.0 or 1.0)
	frame1_.set_label_align( 0.0, 0.25 );
	frame2_.set_label_align( 1.0 );
  	frame1_.set_shadow_type( Gtk::ShadowType::SHADOW_ETCHED_OUT ); // set the style of the frame
  	frame2_.set_shadow_type( Gtk::ShadowType::SHADOW_ETCHED_IN );

  	// set button listeners
  	button1_.signal_clicked().connect( sigc::mem_fun(*this, &MyWindow::b1_clicked) );
  	button2_.signal_clicked().connect( sigc::mem_fun(*this, &MyWindow::b2_clicked) );

  	add_events( Gdk::KEY_PRESS_MASK );

	show_all_children();
} // MyWindow::MyWindow

MyWindow::~MyWindow() {}

bool MyWindow::on_key_press_event( GdkEventKey* keyEvent ) {
	static int numTimesReturnPressed = 0;
	if ( keyEvent->keyval == GDK_KEY_Return ) {
		numTimesReturnPressed++;
		string s = valueField_.get_text();
		cout << "User entered: " << s << endl;
		label_.set_text( s );
		valueField_.set_text( std::to_string( numTimesReturnPressed ) );
		return true;
	} // if

	// return true to indicate handled; otherwise call the base class
	return Gtk::Window::on_key_press_event( keyEvent );
}

void MyWindow::b1_clicked() {
	index_++;
	index_ %= imageNames_.size();
	image_.set( imageNames_.at( index_ ) );
}

void MyWindow::b2_clicked() {
	string s = valueField_.get_text();
	cout << "old text = " << s << endl;
	valueField_.set_text( "yyy" );
}