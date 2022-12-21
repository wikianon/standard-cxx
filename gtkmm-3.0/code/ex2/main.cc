#include <gtkmm/application.h>	// GTK::Application
#include "window.h"
#include <iostream>

int main( int argc, char * argv[] ) {
	// Create the default application to run.
	auto app = Gtk::Application::create( argc, argv, "GTKmm.Tutorial.Example2" );  
  
	MyWindow window;  	
	std::cout << "waiting for window to close" << std::endl;        
	return app->run( window );	
} // main
