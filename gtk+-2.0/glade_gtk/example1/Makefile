# change application name here (executable output name)
TARGET=getname
 
# compiler
CC=gcc
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

# linker
LDFLAGS=$(PTHREAD) $(GTK2LIB) -export-dynamic
 
OBJS=$(TARGET).o
 
all: $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)
	
$(TARGET).o: $(TARGET).c

	$(CC) -c $(CCFLAGS) $(TARGET).c $(GTK2LIB)

exec:
	./$(TARGET)
	
clean:
	rm -rf *.o  $(TARGET)