#include <gtkmm/application.h>	// Gtk::Application
#include <gtkmm/window.h>	    // Gtk::Window
#include <iostream>

int main( int argc, char * argv[] ) {
	// Create the default application to run.
	auto app = Gtk::Application::create( argc, argv, "GTKmm.Tutorial.Example1" );  
  
	Gtk::Window window;                   // Create a basic window. 	
	window.set_title( "Example 1" );      // Sets the window title.
	window.set_default_size( 500, 200 );  // Set default size, width and height, in pixels.

	std::cout << "waiting for window to close" << std::endl;        
	return app->run( window );	
} // main
