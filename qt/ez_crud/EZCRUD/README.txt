Original file by Luis Valdes <luisvaldes88@gmail.com>
Copyright(C) 2010, 2011, 2012

This is EZCRUD version 0.4

Easy CRUD is an application arquitecture that can be extended through external modules (plugins) using Qt.
This arquitecture is designed to provide the basic tools to create CRUD forms, Configuration forms and Report forms.
Also it includes the root module (mod_root), its function is to allow the ROOT user to control the access privileges of thevsystem's user, the ROOT user can limit the normal users privileges, this is useful for systems that requires security levels.

INSTALLING EZCRUD

You need to compile Poco C++ Libraries and configure the include path and lib path in the .pro file of the project

The default configuration of the application demo is:

User: ROOT
Password: 12345

DEMOS AND EXAMPLES

You can download an example for windows here: 
http://sourceforge.net/projects/easy-crud/files/windows/setup.exe/download
I am working on a debian package for Linux, it will be available very soon.

REFERENCE DOCUMENTATION

I am STILL working on this.


SUPPORTED PLATFORMS

Already tested on:
* Linux 32/64 compiled with gcc 4.6 
* Windows XP / Vista / 7 compiled with mingw32 4.4