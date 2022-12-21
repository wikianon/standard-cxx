#ifndef MBPLUGIN_H
#define MBPLUGIN_H

#define MB_INBOX 1
#define MB_SAVED 2
#define MB_SENT 3
#define MB_TRASH 4

//no op is used to signal no change.
#define MB_NO_OP 0

#define MB_NOT_REPLIED 1
#define MB_REPLIED 2

#define MB_NOT_READ 1
#define MB_READ 2

#define MB_HIDDEN 1
#define MB_NOT_HIDDEN 2

struct mb_message
{
	//headers
	//headers my or may not have \n at the end
	char* to;
	char* from;
	char* date;
	char* reply_to;
	char* cc;
	char* subject;
	char* return_receipt_to;
	int read;
	int reply;
	int ismime;

	char* message;
	char* attach[100]; //zero is not used
	int num_attach; //number of attachments
};

struct mb_message_header
{
	char *to;
	char *from;
	char *date;
	char *subject;
	int read;
	int reply;
	int id_number;  //message id number
	int hidden;	//system message not ment to be shown
};
//from most recent to the oldest
struct mb_list //linked list
{
	struct mb_list *next;
	struct mb_message_header *data;
	int total_messages;  //quicker then going through the list
			 //not including hidden messages!!!!!
			//only message header total message is correct
			//sure thats it... ummmm, yes!!!!
			//ok, fine its bad programming
			//so shoot me, see if I care.
};

#endif
