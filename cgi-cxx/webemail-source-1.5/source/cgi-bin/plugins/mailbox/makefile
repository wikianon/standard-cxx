all: mbplugin.local.so

mbplugin.local.so: mbplugin.local.o fullpipe.o
	gcc -shared -o mbplugin.local.so fullpipe.o \
mbplugin.local.o

mbplugin.local.o: mbplugin.local.c ../../mbplugin.h ../../plugin.h \
fullpipe.h
	gcc -fPIC -c mbplugin.local.c

fullpipe.o: fullpipe.c
	gcc -fPIC -c fullpipe.c


clean:
	rm -f *.o
	rm -f *.so

