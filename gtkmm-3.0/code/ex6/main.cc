#include <gtkmm.h>
#include "window.h"
#include <iostream>

int main( int argc, char * argv[] ) {
	try {
		auto app = Gtk::Application::create( argc, argv, "gtkmm.tutorial.example6" );
		auto builder_ = Gtk::Builder::create_from_file( "gui.glade" );

		MyWindow myWindow_( app, builder_ );
		app->run( myWindow_ );
		app->remove_window( myWindow_ );

	}  catch( const Glib::FileError & ex ) {
		std::cerr << "FileError: " << ex.what() << std::endl;
		return 1;
	} catch( const Glib::MarkupError & ex ) {
		std::cerr << "MarkupError: " << ex.what() << std::endl;
		return 1;
	} catch( const Gtk::BuilderError & ex ) {
		std::cerr << "BuilderError: " << ex.what() << std::endl;
		return 1;
    } // catch
    
    return 0;
} // main
