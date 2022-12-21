# This is the makefile for nmail
# to set installation directories, see config.h


CC = gcc
CFLAGS = -Wall -O2 -s

all: nmail login_validate

clean: 
	rm -f nmail login_validate core *.o

install: nmail login_validate
	./install.sh


login_validate: login_validate.cpp
	$(CC) $(CFLAGS) -o login_validate login_validate.cpp -lcrypt

nmail: main.o environment.o headers.o parse_form.o \
	   login_authenticate.o session.o \
	   login_page.o inbox_page.o logout_page.o err_page.o \
	   parse_mail_headers.o view_message.o mail_process.o \
	   delete_message.o view_attachment.o encoding_conv.o \
	   preferences.o edit_addr_book_page.o edit_addr_book.o \
	   compose_message_page.o view_addresses.o send_message.o \
	   random.o custom_string.o addr.o import_addr_page.o \
	   do_import_addr.o conv_csv.o conv_ldif.o export_addr.o \
	   edit_pref_page.o action.o
	$(CC) $(CFLAGS) -o nmail main.o environment.o headers.o  \
	   parse_form.o login_authenticate.o session.o \
	   login_page.o inbox_page.o logout_page.o err_page.o \
	   parse_mail_headers.o view_message.o mail_process.o \
	   delete_message.o view_attachment.o encoding_conv.o \
	   preferences.o edit_addr_book_page.o edit_addr_book.o \
	   compose_message_page.o view_addresses.o send_message.o \
	   random.o custom_string.o addr.o import_addr_page.o \
	   do_import_addr.o conv_csv.o conv_ldif.o export_addr.o \
	   edit_pref_page.o action.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

environment.o: environment.cpp
	$(CC) $(CFLAGS) -c environment.cpp

headers.o: headers.cpp
	$(CC) $(CFLAGS) -c headers.cpp

parse_form.o: parse_form.cpp
	$(CC) $(CFLAGS) -c parse_form.cpp

session.o: session.cpp
	$(CC) $(CFLAGS) -c session.cpp

login_authenticate.o: login_authenticate.cpp
	$(CC) $(CFLAGS) -c login_authenticate.cpp

login_page.o: login_page.cpp
	$(CC) $(CFLAGS) -c login_page.cpp

inbox_page.o: inbox_page.cpp
	$(CC) $(CFLAGS) -c inbox_page.cpp

logout_page.o: logout_page.cpp
	$(CC) $(CFLAGS) -c logout_page.cpp

err_page.o: err_page.cpp
	$(CC) $(CFLAGS) -c err_page.cpp

parse_mail_headers.o: parse_mail_headers.cpp
	$(CC) $(CFLAGS) -c parse_mail_headers.cpp

view_message.o: view_message.cpp
	$(CC) $(CFLAGS) -c view_message.cpp

view_attachment.o: view_attachment.cpp
	$(CC) $(CFLAGS) -c view_attachment.cpp

mail_process.o: mail_process.cpp
	$(CC) $(CFLAGS) -c mail_process.cpp

delete_message.o: delete_message.cpp
	$(CC) $(CFLAGS) -c delete_message.cpp

encoding_conv.o: encoding_conv.cpp
	$(CC) $(CFLAGS) -c encoding_conv.cpp

preferences.o: preferences.cpp
	$(CC) $(CFLAGS) -c preferences.cpp

edit_addr_book_page.o: edit_addr_book_page.cpp
	$(CC) $(CFLAGS) -c edit_addr_book_page.cpp

edit_addr_book.o: edit_addr_book.cpp
	$(CC) $(CFLAGS) -c edit_addr_book.cpp

compose_message_page.o: compose_message_page.cpp
	$(CC) $(CFLAGS) -c compose_message_page.cpp

view_addresses.o: view_addresses.cpp
	$(CC) $(CFLAGS) -c view_addresses.cpp

send_message.o: send_message.cpp
	$(CC) $(CFLAGS) -c send_message.cpp

random.o: random.cpp
	$(CC) $(CFLAGS) -c random.cpp

custom_string.o: custom_string.cpp
	$(CC) $(CFLAGS) -c custom_string.cpp

addr.o: addr.cpp
	$(CC) $(CFLAGS) -c addr.cpp

import_addr_page.o: import_addr_page.cpp
	$(CC) $(CFLAGS) -c import_addr_page.cpp

do_import_addr.o: do_import_addr.cpp
	$(CC) $(CFLAGS) -c do_import_addr.cpp

conv_csv.o: conv_csv.cpp
	$(CC) $(CFLAGS) -c conv_csv.cpp

conv_ldif.o: conv_ldif.cpp
	$(CC) $(CFLAGS) -c conv_ldif.cpp

export_addr.o: export_addr.cpp
	$(CC) $(CFLAGS) -c export_addr.cpp

edit_pref_page.o: edit_pref_page.cpp
	$(CC) $(CFLAGS) -c edit_pref_page.cpp

action.o: action.cpp
	$(CC) $(CFLAGS) -c action.cpp


