#
# makefile for efone
#

TARGET=efone
SRCS=main.c server.c nethelp.c client.c trans.c soundcard.c
HDRS=efone.h server.h nethelp.h	client.h trans.h soundcard.h
MISCFILES=TODO README COPYING
MAKEFILE=Makefile
OBJS=${SRCS:.c=.o}

CC=gcc
CFLAGS=-c -g -Wall -D_REENTRANT
LD=gcc -lpthread -lreadline
LDFLAGS=-g -o ${TARGET}

all: efone
	
efone: ${OBJS}
	${LD} ${LDFLAGS} ${OBJS} 

clean:
	rm -f ${OBJS} ${TARGET} *~ core

co:
	cd .. ; cvs checkout efone ; cd efone

cm:
	cvs commit

dist:
	cd .. ; tar -zcvf efone.tar.gz efone/*.c efone/*.h efone/Makefile efone/TODO efone/README efone/COPYING efone/Makefile efone/ChangeLog ; cd efone
