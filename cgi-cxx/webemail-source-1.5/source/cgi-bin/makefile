
all: email.cgi
	cd plugins/display; make; cd ../..
	cd plugins/mailbox; make; cd ../..
	cd plugins/address; make; cd ../..

email.cgi: email.o address.o display.o security.o utildisplay.o delete.o \
send.o utils.o signals.o bothlock.o save.o fullpipe.o sent.o \
attachments.o inbox.o pluginctl.o readconfig.o
	g++ -O -o email.cgi email.o address.o display.o security.o \
utildisplay.o delete.o send.o utils.o signals.o bothlock.o save.o \
fullpipe.o sent.o attachments.o inbox.o pluginctl.o readconfig.o \
-lcrypt -ldl -lrt

email.o: email.cpp webemail.h displugin.h plugin.h address.h
	g++ -O -c email.cpp

address.o: address.cpp webemail.h
	g++ -O -c address.cpp 

display.o: display.cpp webemail.h readconfig.h
	g++ -O -c display.cpp

security.o: security.cpp security.h webemail.h
	g++ -O -c security.cpp

utildisplay.o: utildisplay.cpp webemail.h
	g++ -O -c utildisplay.cpp

delete.o: delete.cpp webemail.h address.h
	g++ -O -c delete.cpp

send.o: send.cpp webemail.h
	g++ -O -c send.cpp

utils.o: utils.cpp webemail.h
	g++ -O -c utils.cpp

signals.o: signals.cpp webemail.h
	g++ -O -c signals.cpp

bothlock.o: bothlock.cpp bothlock.h webemail.h
	g++ -O -c bothlock.cpp 

save.o: save.cpp save.h webemail.h
	g++ -O -c save.cpp

sent.o: sent.cpp sent.h webemail.h
	g++ -O -c sent.cpp

attachments.o: attachments.cpp attachments.h webemail.h
	g++ -O -c attachments.cpp

inbox.o: inbox.cpp inbox.h webemail.h
	g++ -O -c inbox.cpp

fullpipe.o: fullpipe.cpp fullpipe.h
	g++ -O -c fullpipe.cpp

pluginctl.o: pluginctl.cpp pluginctl.h webemail.h
	g++ -O -c pluginctl.cpp

readconfig.o: readconfig.cpp readconfig.h
	g++ -O -c readconfig.cpp

clean:
	rm -f *.o
	rm -f *.cgi
	cd plugins/display; make clean; cd ../..
	cd plugins/mailbox; make clean; cd ../..
	cd plugins/address; make clean; cd ../..
