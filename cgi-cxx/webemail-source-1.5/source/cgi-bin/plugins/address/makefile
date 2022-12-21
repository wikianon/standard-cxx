all: addrplugin.old.so

addrplugin.old.so: addrplugin.old.o
	gcc -shared -o addrplugin.old.so addrplugin.old.o
addrplugin.old.o: addrplugin.old.c ../../addrplugin.h ../../plugin.h
	gcc -fPIC -c addrplugin.old.c

clean:
	rm -f *.o
	rm -f *.so

