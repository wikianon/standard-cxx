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

MyWindow::MyWindow( const Glib::RefPtr<Gtk::Application>& app ) :
    app_{app}, 
    mainBox_{Gtk::Orientation::ORIENTATION_VERTICAL},
    paned_{Gtk::Orientation::ORIENTATION_VERTICAL},
    label_{"Enter text: "},
    image_{ MyWindow::imageNames_.at( index_ ) }
{
    set_title( "Example 5" );      // Sets the window title.
    set_default_size( 500, 300 );  // Set default size, width and height, in pixels.
    set_border_width( 10 );
    add( mainBox_ );
    setUpMenu();
    structureGUI();
} // MyWindow::MyWindow

MyWindow::~MyWindow() {
} // MyWindow::~MyWindow

void MyWindow::setUpMenu() {
    actionGroup_ = Gio::SimpleActionGroup::create();
    builder_ = Gtk::Builder::create();
    insert_action_group( "example", actionGroup_ ); // add action group to window

    // Tie handlers to menu choices.
    actionGroup_->add_action( "sub_act_1.1", sigc::mem_fun( *this, &MyWindow::sub_action1_1 ) );
    actionGroup_->add_action( "sub_act_1.2", sigc::mem_fun( *this, &MyWindow::sub_action1_2 ) );
    actionGroup_->add_action( "quit", sigc::mem_fun( *this, &MyWindow::sub_action1_quit ) );
    actionGroup_->add_action( "sub_act_2.1", sigc::mem_fun( *this, &MyWindow::sub_action2_1 ) );
    actionGroup_->add_action( "sub_act_2.2", sigc::mem_fun( *this, &MyWindow::sub_action2_2 ) );

    // Tie handlers to toolbar buttons
    actionGroup_->add_action( "show1", sigc::mem_fun( *this, &MyWindow::show_image_1 ) );
    actionGroup_->add_action( "show2", sigc::mem_fun( *this, &MyWindow::show_image_2 ) );
    actionGroup_->add_action( "show3", sigc::mem_fun( *this, &MyWindow::show_image_3 ) );

    // Use XML to specify the menu layout
    const char* ui_info =
	"<interface>"
	"  <menu id='menubar'>"
	"    <submenu>"
	"      <attribute name='label' translatable='yes'>_ActionGroup1</attribute>"
	"      <section>"
	"        <item>"
	"          <attribute name='label' translatable='yes'>_Sub1.1</attribute>"
	"          <attribute name='action'>example.sub_act_1.1</attribute>"
	"          <attribute name='accel'>&lt;Primary&gt;1</attribute>"
	"        </item>"
	"        <item>"
	"          <attribute name='label' translatable='yes'>_Sub1.2</attribute>"
	"          <attribute name='action'>example.sub_act_1.2</attribute>"
	"          <attribute name='accel'>&lt;Primary&gt;2</attribute>"
	"        </item>"
	"      </section>"
	"      <section>"
	"        <item>"
	"          <attribute name='label' translatable='yes'>_Quit</attribute>"
	"          <attribute name='action'>example.quit</attribute>"
	"          <attribute name='accel'>&lt;Primary&gt;q</attribute>"
	"        </item>"
	"      </section>"
	"    </submenu>"
	"    <submenu>"
	"      <attribute name='label' translatable='yes'>_ActionGroup2</attribute>"
	"      <item>"
	"        <attribute name='label' translatable='yes'>_Sub2.1</attribute>"
	"        <attribute name='action'>example.sub_act_2.1</attribute>"
	"        <attribute name='accel'>&lt;Primary&gt;3</attribute>"
	"      </item>"
	"      <item>"
	"        <attribute name='label' translatable='yes'>_Sub2.2</attribute>"
	"        <attribute name='action'>example.sub_act_2.2</attribute>"
	"        <attribute name='accel'>&lt;Primary&gt;4</attribute>"
	"      </item>"
	"    </submenu>"
	"  </menu>"
	"</interface>";

    // When the menubar is a child of a Gtk::Window, keyboard accelerators are not
    // automatically fetched from the Gio::Menu.
    // See the examples/book/menus/main_menu example for an alternative way of
    // adding the menubar when using Gtk::ApplicationWindow.
    // Gtk::Application::set_accel_for_action() is new in gtkmm 3.11.9.
    app_->set_accel_for_action("example.sub_act_1.1", "<Primary>1");
    app_->set_accel_for_action("example.sub_act_1.2", "<Primary>2");
    app_->set_accel_for_action("example.quit", "<Primary>q");
    app_->set_accel_for_action("example.sub_act_2.1", "<Primary>3");
    app_->set_accel_for_action("example.sub_act_2.2", "<Primary>4");

    try {
	builder_->add_from_string( ui_info );
    } catch( const Glib::Error& ex) {
	std::cerr << "Building menu and toolbar failed: " <<  ex.what();
    }

    // Get the menubar:
    auto object = builder_->get_object( "menubar" );
    auto gmenu = Glib::RefPtr<Gio::Menu>::cast_dynamic( object );

    // Did the conversion to a Gio::Menu fail, resulting in a nullptr?
    if ( !gmenu ) g_warning( "Menu not found" );
    else {
	auto menuBar = Gtk::manage( new Gtk::MenuBar( gmenu ) );
	mainBox_.pack_start( *menuBar, Gtk::PACK_SHRINK ); //Add the MenuBar to the window:

	// Find the defined "toolbar" and add.
	builder_->add_from_file( "toolbar.glade" );
	Gtk::Toolbar * toolbar = nullptr;
	builder_->get_widget( "toolbar", toolbar );
	if ( !toolbar )
	    g_warning( "toolbar not found" );
	else
	    mainBox_.pack_start( *toolbar, Gtk::PACK_SHRINK);
    } // if
} // MyWindow::setUpMenu

void MyWindow::structureGUI() {
    button1_.set_image( image_ );
    button2_.set_label( "something" );
    bbox_.add( button1_ );
    bbox_.add( button2_ );
    valueField_.set_text( "xxx" );
    box_.add( label_ );
    box_.add( valueField_ );

    frame1_.add( bbox_ );
    frame2_.add( box_ );	
    paned_.add1( frame1_ );
    paned_.add2( frame2_ );
    mainBox_.add( paned_ );

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
} // MyWindow::structureGUI

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
} // MyWindow::on_key_press_event

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

void MyWindow::sub_action1_1() { cout << "sub action 1.1" << endl; }
void MyWindow::sub_action1_2() { cout << "sub action 1.2" << endl; }
void MyWindow::sub_action1_quit() {
    Gtk::MessageDialog dialog( *this, "Termination Dialog", false, 
			       Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
    dialog.set_secondary_text( "Are you *really* sure you want to Quit?" );
    int result = dialog.run();
    switch(result) {
      case( Gtk::RESPONSE_OK ):
	  hide(); //Closes the main window to stop the app->run().
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
void MyWindow::show_image_1() { image_.set( imageNames_.at( 0 ) ); }
void MyWindow::show_image_2() { image_.set( imageNames_.at( 1 ) ); }
void MyWindow::show_image_3() { image_.set( imageNames_.at( 2 ) ); }
