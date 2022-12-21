obj-m := hello.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

CHECKFLAGS := -D__linux__ -Dlinux -D__STDC__ -Dunix -D__unix__ \
		  -Wbitwise -Wno-return-void

default:
	make -C $(KDIR) SUBDIRS=$(PWD) modules 

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

	
	
#obj-m += hello.o

#all:
#    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

#clean:
#      make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean