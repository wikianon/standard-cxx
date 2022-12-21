all: displugin.classic.so displugin.text.so displugin.file.so

displugin.classic.so: displugin.classic.o
	gcc -shared -o displugin.classic.so displugin.classic.o
displugin.classic.o: displugin.classic.c ../../displugin.h ../../plugin.h \
../../addrplugin.h
	gcc -fPIC -c displugin.classic.c


displugin.text.so: displugin.text.o
	gcc -shared -o displugin.text.so displugin.text.o
displugin.text.o: displugin.text.c ../../displugin.h ../../plugin.h \
../../addrplugin.h
	gcc -fPIC -c displugin.text.c


displugin.file.so: displugin.file.o
	gcc -shared -o displugin.file.so displugin.file.o
displugin.file.o: displugin.file.c ../../displugin.h ../../plugin.h \
../../addrplugin.h
	gcc -fPIC -c displugin.file.c


clean:
	rm -f *.o
	rm -f *.so

