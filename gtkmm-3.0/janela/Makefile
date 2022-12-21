# change application name here (executable output name)
TARGET=janela
 
# compiler
CC=g++
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall
 
PTHREAD=-pthread
 
CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe
 
GTK3LIB=`pkg-config --cflags --libs gtk+-3.0`

GTK2LIB=`pkg-config --cflags --libs gtk+-2.0 gmodule-2.0`

GTKMM=`pkg-config --cflags --libs gtkmm-3.0`

# linker
LDFLAGS=$(PTHREAD) $(GTKMM) -export-dynamic
 
OBJS=$(TARGET).o
 
all: $(OBJS)

	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)
	
$(TARGET).o: $(TARGET).cc

	$(CC) -c $(CCFLAGS) $(TARGET).cc $(GTKMM)

exec:
	./$(TARGET)
	
clean:
	rm -rf *.o  $(TARGET)