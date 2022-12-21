all:	MAILER \
	foldermgr.cgi message.cgi deliver folder.cgi preferences.cgi \
	template.cgi addressbook.cgi raw.cgi download.cgi rfc822.cgi \
	forward.cgi compose.cgi reply.cgi post.cgi login.cgi

LIB=	Compose.o Message.o Address.o BookAddress.o Layout.o \
	MessageBody.o ShowFolder.o Folder.o FolderEntry.o \
	ShowMessage.o Post.o Preferences.o \
	FolderMgr.o Template.o AddressBook.o \
	Text.o Deliver.o Recipient.o \
	Login.o

SRC=	Compose.cc \
	Address.cc Address.h BookAddress.cc BookAddress.h \
	Layout.cc Layout.h MessageBody.cc MessageBody.h \
	Message.h Message.cc \
	ShowFolder.cc Folder.cc Folder.h \
	FolderEntry.cc FolderEntry.h ShowMessage.cc \
	Post.cc Post.h Preferences.cc \
	FolderMgr.cc Template.cc \
	AddressBook.cc \
	Text.cc Text.h \
	Deliver.cc Recipient.cc Recipient.h \
	Login.cc \
	Timebuf.h Config.h Register.h main.cc

TEXT=	INSTALL README LICENSE install.sh adduser.sh

OTHER=	Config.local Makefile

ALL=	main.o $(LIB)

clean:
	rm -f *o

mail.tgz:	$(SRC) $(OTHER) $(TEXT)
	tar czf mail.tgz $(SRC) $(OTHER) $(TEXT)

rcs:	$(SRC) Makefile Config.local
	ci -l -t- $(SRC) Makefile Config.local

MAILER:	$(ALL)
	c++ -o MAILER main.o $(LIB)  -LCGI++ -lcgi++ -lg++
	strip MAILER
	chmod 4755 MAILER
	ln -f MAILER login.cgi
	ln -f MAILER template.cgi
	ln -f MAILER browser.cgi
	ln -f MAILER deliver
	ln -f MAILER foldermgr.cgi
	ln -f MAILER forward.cgi
	ln -f MAILER addressbook.cgi
	ln -f MAILER folder.cgi
	ln -f MAILER message.cgi
	ln -f MAILER rfc822.cgi
	ln -f MAILER download.cgi
	ln -f MAILER raw.cgi
	ln -f MAILER post.cgi
	ln -f MAILER preferences.cgi
	ln -f MAILER compose.cgi
	ln -f MAILER reply.cgi

################MakeMake auto#############################
Address.o:	Address.cc
	c++ -O2 -Wall -c Address.cc
Address.cc:	Address.h BookAddress.h
	touch Address.cc

AddressBook.o:	AddressBook.cc
	c++ -O2 -Wall -c AddressBook.cc
AddressBook.cc:	BookAddress.h Config.h Layout.h Register.h
	touch AddressBook.cc

BookAddress.o:	BookAddress.cc
	c++ -O2 -Wall -c BookAddress.cc
BookAddress.cc:	Config.h BookAddress.h
	touch BookAddress.cc

BookAddress.h:	Address.h
	touch BookAddress.h

Compose.o:	Compose.cc
	c++ -O2 -Wall -c Compose.cc
Compose.cc:	Config.h Message.h Register.h Layout.h
	touch Compose.cc

Config.h:	Config.local
	touch Config.h

Deliver.o:	Deliver.cc
	c++ -O2 -Wall -c Deliver.cc
Deliver.cc:	Message.h Config.h Recipient.h Register.h
	touch Deliver.cc

Folder.o:	Folder.cc
	c++ -O2 -Wall -c Folder.cc
Folder.cc:	Folder.h FolderEntry.h Config.h
	touch Folder.cc

FolderEntry.o:	FolderEntry.cc
	c++ -O2 -Wall -c FolderEntry.cc
FolderEntry.cc:	FolderEntry.h
	touch FolderEntry.cc

FolderMgr.o:	FolderMgr.cc
	c++ -O2 -Wall -c FolderMgr.cc
FolderMgr.cc:	Config.h Layout.h Register.h
	touch FolderMgr.cc

Layout.o:	Layout.cc
	c++ -O2 -Wall -c Layout.cc
Layout.cc:	Config.h Layout.h Timebuf.h
	touch Layout.cc

Login.o:	Login.cc
	c++ -O2 -Wall -c Login.cc
Login.cc:	Layout.h Register.h
	touch Login.cc

Message.o:	Message.cc
	c++ -O2 -Wall -c Message.cc
Message.cc:	Message.h Address.h Config.h
	touch Message.cc

Message.h:	MessageBody.h
	touch Message.h

MessageBody.o:	MessageBody.cc
	c++ -O2 -Wall -c MessageBody.cc
MessageBody.cc:	MessageBody.h
	touch MessageBody.cc

Post.o:	Post.cc
	c++ -O2 -Wall -c Post.cc
Post.cc:	Config.h Message.h Layout.h Address.h Register.h Post.h Text.h Timebuf.h
	touch Post.cc

Post.h:	Message.h
	touch Post.h

Preferences.o:	Preferences.cc
	c++ -O2 -Wall -c Preferences.cc
Preferences.cc:	Config.h Layout.h Register.h
	touch Preferences.cc

Recipient.o:	Recipient.cc
	c++ -O2 -Wall -c Recipient.cc
Recipient.cc:	BookAddress.h Post.h Message.h Config.h Recipient.h
	touch Recipient.cc

Recipient.h:	Message.h
	touch Recipient.h

ShowFolder.o:	ShowFolder.cc
	c++ -O2 -Wall -c ShowFolder.cc
ShowFolder.cc:	Folder.h FolderEntry.h Config.h Layout.h Register.h
	touch ShowFolder.cc

ShowMessage.o:	ShowMessage.cc
	c++ -O2 -Wall -c ShowMessage.cc
ShowMessage.cc:	Message.h Text.h Address.h BookAddress.h Config.h Layout.h Register.h
	touch ShowMessage.cc

Template.o:	Template.cc
	c++ -O2 -Wall -c Template.cc
Template.cc:	Config.h Layout.h Register.h
	touch Template.cc

Text.o:	Text.cc
	c++ -O2 -Wall -c Text.cc
Text.cc:	Text.h
	touch Text.cc

main.o:	main.cc
	c++ -O2 -Wall -c main.cc
main.cc:	Register.h Config.h
	touch main.cc

