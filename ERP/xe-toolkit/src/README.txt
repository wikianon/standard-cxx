** Introduction

This is the top level of the build tree for The XE Toolkit.

If you are reading this file as delivered with a pre-built package,
the source code for XE can be found at

http://sourceforge.net/projects/xe-toolkit

and the source for the 3rd party packages that XE uses can be found
at the locations shown below.

Additionally, the net-snmp library can be found at

http://net-snmp.sourceforge.net

If you are using XE on AIX, you will need the SNMP MIB files contained
within the net-snmp package. Download the net-snmp package and unarchive
the tarball. Under the top level directory (net-snmp-5.4.1, for instance)
there is a directory called "mibs". You should recursively copy this
directory to the "etc" directory at the top level of the XE installation.
CaptiveCollector will not run correctly without these files.

** Prerequisite Jar Files

Before you can build, you must download and install the following jar
files in the top level 'jars' directory.

looks.jar     http://www.jgoodies.com/downloads/libraries.html
forms.jar     http://www.jgoodies.com/downloads/libraries.html
Chart2D.jar   http://chart2d.sourceforge.net

Out of respect to the developers, these are not deep links to the jar
files. Please take a moment to look at their sites and consider the
immense effort that they put forth to develop these products.

The downloads are zip files containing the source and jar file. The
'looks' and 'forms' jar files have revisions attached. They should
be copied to the top level 'jars' directory without the revision,
as looks.jar and forms.jar. The Chart2D.jar file has no revision and
can be copied as is.

** Understanding the XE Build Process

The XE Toolkit is built using a script called 'snap'. Snap is a
shell script that reads the file "Snapfile" and generates a Makefile for
the build and then invokes 'make'. See the snap documentation for
more information (TBD). The snap source code is in the 'snap' directory
at this level. Invoking 'snap/snap' at this level will build the
C++ code for the current platform. The snap command can be used
as if it were the 'make' command. A 'snap clean' will do the same
as a 'make clean' would if there were a Makefile.

The java directory has its own Snapfile for building the Java code.
Running 'snap all' is the quickest way to build the Java code as it
invokes the Java compiler with all of the source files at once. To
build the jar files and install them in the jars directory, run
'snap all install' from the java directory.

** Building the Product on Unix

There is JNI code in the build tree. The compiler must know where the
Java include files for jni are located. The Snapfile in the JNI directories
default to /opt/java for the path to the top of the Java install. If your
installation is elsewhere (and short of creating a symbolic link in /opt that
points to the Java install) invoke snap with

    java_top=/path/to/java snap install

for instance, if /usr/java is the Java directory or a symbolic link to
a jdk, then run

    java_top=/usr/java snap install

and if you have gnu make installed on a dual or quad core machine,
try

    java_top=/usr/java make=gmake snap -j 2 install

but I digress.

1) Get snap in your path. Either modify the path or alias it or whatever.
2) Do this:

    snap install
    cd java
    snap all install

At this point, the libs and jars for this platform will be in the
directories at the top level. You may then go to the top level and
do this:

    cd init.d
    CaptiveCollector start
    cd ../bin
    Top

and you should be in business.

** Building the Product on Windows

The Windows code uses Makefiles (ala nmake). The product is built
using Microsoft Visual Studio 8 (2005). All of the Makefiles are written
to use VS8 tools, such as cl, link and mt. If you try using anything else,
you are completely on your own. Also know that any checkins of modified
Makefiles to use other compilers will be bounced.

Anyway, using the VS8 command prompt, from this directory, run

    nmake install

or if you're just using a regular cmd window, run

    build install

which will use the build.bat file which has all of the variables set
for VS8 (in their location on my build machine). Your mileage may vary.

All of the native (C++) code will be built and installed in the
correct place. There is no mechanism for compiling the Java code on
Windows. I suppose this is a shortcoming. If someone is motivated
to take one of the snap-generated Makefiles and make an equivalent
make.win file for Windows, then by all means, be my guest.

** What to do After Build

Once the build is done, the top level of the tree looks enough like the
installed product to work. Just "tar cf - . | (cd there ; tar xf -)"
and away you go.

Good luck and have fun.
