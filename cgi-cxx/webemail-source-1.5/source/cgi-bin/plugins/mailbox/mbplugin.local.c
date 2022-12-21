#include "../../plugin.h"
#include "../../mbplugin.h"
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "../../fullpipe.h"

#define MB_L_NOHEADER 0
#define MB_L_FROM 1
#define MB_L_TO 2
#define MB_L_DATE 3
#define MB_L_CC 4
#define MB_L_REPLYTO 5
#define MB_L_SUBJECT 6
#define MB_L_RETURNRECEIPTTO 7

int lock_flock(FILE *file);
int unlock_flock(FILE *file);

int unlock_fcntl(FILE *file);
int lock_fcntl(FILE *file, char type);


//send error messages to stderr
//format webemail:mb:{plugin name}:{function} error message


//default plugin interface
int get_interface_version()
{
	return 2;
}
int get_plugin_subsystem()
{
	return 2;  //mail box sub system.
}
double get_plugin_version()
{
	return 0.9;
}
char* get_plugin_name()
{
	char *name;
	name = malloc(20);
	strncpy(name, "Local", 19);
	return name;
}
//frees allocated memory
void free_mem(void *data)
{
	free(data);
	return;
}
//plugin must handle message freeing, since points could point to
//a memory location or heap.  only the plugin knows
void free_message(struct mb_message* message)
{
	int i = 1;

	if (message->to != NULL)
		free(message->to);
	if (message->from != NULL)
		free(message->from);
	if (message->date != NULL)
		free(message->date);
	if (message->reply_to != NULL)
		free(message->reply_to);
	if (message->cc != NULL)
		free(message->cc);
	if (message->subject != NULL)
		free(message->subject);
	if (message->return_receipt_to != NULL)
		free(message->return_receipt_to);

	if (message->message != NULL)
		free(message->message);
	for (i = 1; i <= message->num_attach; i++)
		free(message->attach[i]);
	return;
}
void free_message_list(struct mb_list* list)
{
	struct mb_list* current;
	struct mb_list* next;
	current = list;

	while(current != NULL)
	{
		if (current->data != NULL)
		{
			//delete message header
			if (current->data->to != NULL)
				free(current->data->to);
			if (current->data->from != NULL)
				free(current->data->from);
			if (current->data->date != NULL)
				free(current->data->date);
			if (current->data->subject != NULL)
				free(current->data->subject);
			free(current->data);
		}
		next = current->next;
		free(current);
		current = next;
	}
	return;
}

//---------------------------------------------------
//private functions
//copys text widthing quotes to outside of quotes
char *  guessmime(char filename[])
{
	int i = 0;
	int ext = 0;
	while (filename[i] != '\0' && filename[i] != '\n')
	{
		if (filename[i] == '.')
		{
			ext = i;
		}
		i++;
	}

	if (strncasecmp(".jpeg", filename+ext, 5) == 0 ||
	strncasecmp(".jpg", filename+ext, 4) == 0 ||
	strncasecmp(".jpe", filename+ext, 4) == 0)
		return "image/jpeg";

	//defined at the bottom.
	//if (strncasecmp(".html", filename+ext, 5) == 0 ||
	//strncasecmp(".htm", filename+ext, 4) == 0)
	//	return "text/HTML";

	if (strncasecmp(".mp2", filename+ext, 4) == 0 ||
	strncasecmp(".mpa", filename+ext, 4) == 0 ||
	strncasecmp(".mpega", filename+ext, 6) == 0 ||
	strncasecmp(".mp3", filename+ext, 4) == 0)
		return "audio/x-mpeg";

	if (strncasecmp(".ra", filename+ext, 3) == 0 ||
	strncasecmp(".ram", filename+ext, 4)==0)
		return "audio/x-pn-realaudio-plugin";

	if (strncasecmp(".wpd", filename+ext, 4) == 0)
		return "application/wordperfect5.1";

	if (strncasecmp(".doc", filename+ext, 4) == 0)
		return "application/msword";

	if (strncasecmp(".js", filename+ext, 3) == 0)
		return "application/x-javascript";

	if (strncasecmp(".exe", filename+ext, 4) == 0)
		return "application/octet-stream";

	if (strncasecmp(".jar", filename+ext, 4) == 0)
		return "application/java-archive";

	if (strncasecmp(".avi", filename+ext, 4) == 0)
		return "video/x-msvideo";

	if (strncasecmp(".qt", filename+ext, 3) == 0 ||
	strncasecmp(".mov", filename+ext, 4)==0)
		return "video/quicktime";

	if (strncasecmp(".mpeg", filename+ext, 5) == 0 ||
	strncasecmp(".mpg", filename+ext, 4) == 0 ||
	strncasecmp(".mpe", filename+ext, 4) == 0 )
		return "video/mpeg";

	if (strncasecmp(".wav", filename+ext, 4) == 0)
		return "audio/x-wav";

	if (strncasecmp(".bmp", filename+ext, 4) == 0)
		return "image/x-ms-bmp";

	if (strncasecmp(".xbm", filename+ext, 4) == 0)
		return "image/x-xbitmap";

	if (strncasecmp(".tiff", filename+ext, 5) == 0 ||
	strncasecmp(".tif", filename+ext, 4) == 0)
		return "image/tiff";

	if (strncasecmp(".gif", filename+ext, 4) == 0)
		return "image/gif";

	if (strncasecmp(".rtf", filename+ext, 4) == 0)
		return "application/rtf";

	if (strncasecmp(".txt", filename+ext, 4) == 0 ||
	strncasecmp(".text", filename+ext, 5) == 0)
		return "text/plain";

	if (strncasecmp(".html", filename+ext, 5) == 0 ||
	strncasecmp(".htm", filename+ext, 4) == 0 ||
	strncasecmp(".shtml", filename+ext, 6) == 0)
		return "text/html";

	return "unknown/x-unknown";
}

void removereturn(char * data)
{
	int org = 0;
	int Not = 0;
	while(data[org] != '\0')
	{
		if (data[org] == '\n')
			org++;
		data[Not] = data[org];
		if (data[org] == '\0')
			break;
		org++;
		Not++;
	}
	return;
}

char * inquotes(char * data)
{
	int start = 0;
	int end = 0;
	int i = 1;
	int t;
	if (data[0] == '"')
	{
		start = i;
		while (data[i] != '"' && data[i] != '\0')
		{
			i++;
		}
		end = i;
		for (i = start, t = 0; i < end; i++, t++)
		{
			data[t] = data[i];
		}
		data[end-1] = '\0';
	}

	return data;
}
char * convert(struct plugin_data *data, int mailbox)
{
	char *data_path;
	struct passwd *info;

	data_path = malloc(400);
	info = getpwnam(data->user_name);
	if (info == NULL)
	{
		//bad uid
		return NULL;
	}

	if (mailbox == MB_INBOX)
	{
		snprintf(data_path, 400, "/var/spool/mail/%s", info->pw_name);
	}
	else if (mailbox == MB_SENT)
	{
		snprintf(data_path, 400, "%s/.webmail/sent", info->pw_dir);
	}
	else if (mailbox == MB_SAVED)
	{
		snprintf(data_path, 400, "%s/.webmail/saved", info->pw_dir);
	}
	else if (mailbox == MB_TRASH)
	{
		snprintf(data_path, 400, "%s/.webmail/trash", info->pw_dir);
	}
	else
	{
		free(data_path);
		return NULL; //data error
	}

	return data_path;
}

void set_message_null(struct mb_message *message)
{
	message->to = NULL;
	message->from = NULL;
	message->date = NULL;
	message->reply_to = NULL;
	message->cc = NULL;
	message->subject = NULL;
	message->return_receipt_to = NULL;

	//ints
	message->read = MB_NOT_READ;
	message->reply = MB_NOT_REPLIED;
	message->ismime = 0;

	message->message = NULL;
	//don't need to erase attachments because num_attach
	//message->attach[100];

	//int
	message->num_attach = 0;
	return;
}

int extract_header_info(struct mb_message *message, char data[400])
{
	if (message->from == NULL && strncasecmp(data,"From: ", 6)==0)
	{
		message->from = malloc(strlen(data+6)+1);
		strncpy(message->from, data+6, strlen(data+6)+1);
		return MB_L_FROM;
	}
	else if (message->to == NULL && strncasecmp(data,"To: ", 4)==0)
	{
		message->to = malloc(strlen(data+4)+1);
		strncpy(message->to, data+4, strlen(data+4)+1);
		return MB_L_TO;
	}
	else if (message->date == NULL && strncasecmp(data,"Date: ", 6)==0)
	{
		message->date = malloc(strlen(data+6)+1);
		strncpy(message->date, data+6, strlen(data+6)+1);
		return MB_L_DATE;
	}
	else if (message->cc == NULL && strncasecmp(data,"cc: ", 4)==0)
	{
		message->cc = malloc(strlen(data+4)+1);
		strncpy(message->cc, data+4, strlen(data+4)+1);
		return MB_L_CC;
	}
	else if (message->reply_to == NULL && strncasecmp(data,"Reply-To: ", 10)==0)
	{
		message->reply_to = malloc(strlen(data+10)+1);
		strncpy(message->reply_to, data+10, strlen(data+10)+1);
		return MB_L_REPLYTO;
	}
	else if (message->subject == NULL && strncasecmp(data,"Subject: ",9)==0)
	{
		message->subject = malloc(strlen(data+9)+1);
		strncpy(message->subject, data+9, strlen(data+9)+1);
		return MB_L_SUBJECT;
	}
	else if (message->return_receipt_to == NULL
	&& strncasecmp(data,"Return-Receipt-To: ",18)==0)
	{
		message->return_receipt_to = malloc(strlen(data+18)+1);
		strncpy(message->return_receipt_to, data+18, strlen(data+18)+1);
		return MB_L_RETURNRECEIPTTO;
	}
	else if (strncasecmp(data, "Status: R", 9) ==0)
	{
		message->read = MB_READ;
	}
	else
	{
		return MB_L_NOHEADER;
	}
	return MB_L_NOHEADER;
}
int extract_subheader_info(struct mb_message *message, char data[400], int subheader)
{
	if (message->from != NULL && subheader == MB_L_FROM)
	{
		message->from = realloc(message->from, strlen(data)+strlen(message->from)+1);
		strncpy(message->from+strlen(message->from), data, strlen(data)+1);
		return MB_L_FROM;
	}
	else if (message->to != NULL && subheader == MB_L_TO)
	{
		message->to = realloc(message->to, strlen(data)+strlen(message->to)+1);
		strncpy(message->to+strlen(message->to), data, strlen(data)+1);
		return MB_L_TO;
	}
	else if (message->date != NULL && subheader == MB_L_DATE)
	{
		message->date = realloc(message->date, strlen(data)+strlen(message->date)+1);
		strncpy(message->date+strlen(message->date), data, strlen(data)+1);
		return MB_L_DATE;
	}
	else if (message->cc != NULL && subheader == MB_L_CC)
	{
		message->cc = realloc(message->date, strlen(data)+strlen(message->date)+1);
		strncpy(message->cc+strlen(message->cc), data, strlen(data)+1);
		return MB_L_CC;
	}
	else if (message->reply_to != NULL && subheader == MB_L_REPLYTO)
	{
		message->reply_to = realloc(message->reply_to, strlen(data)+strlen(message->reply_to)+1);
		strncpy(message->reply_to+strlen(message->reply_to), data, strlen(data)+1);
		return MB_L_REPLYTO;
	}
	else if (message->subject != NULL && subheader == MB_L_SUBJECT)
	{
		message->subject = realloc(message->subject, strlen(data)+strlen(message->subject)+1);
		strncpy(message->subject+strlen(message->subject), data, strlen(data)+1);
		return MB_L_SUBJECT;
	}
	else if (message->return_receipt_to != NULL
	&& subheader == MB_L_RETURNRECEIPTTO)
	{
		message->return_receipt_to = realloc(message->return_receipt_to,
		strlen(data)+strlen(message->return_receipt_to)+1);
		strncpy(message->return_receipt_to+strlen(message->return_receipt_to),
		data, strlen(data)+1);
		return MB_L_RETURNRECEIPTTO;
	}
	else
	{
		return MB_L_NOHEADER;
	}
	return MB_L_NOHEADER;
}

void* append_to(char * dest, char *src)
{
	if (dest != NULL)
	{
		dest = realloc(dest, strlen(dest)+strlen(src)+1);
		strncpy(dest+strlen(dest), src, strlen(src)+1);
	}
	else
	{
		dest = malloc(strlen(src)+1);
		strncpy(dest, src, strlen(src)+1);
	}
	return dest;
}

int bothlock(FILE *file, char type)
{
	if (lock_flock(file))
		return 1;
	if (lock_fcntl(file, type))
		return 2;
	return 0;
}

int bothunlock(FILE *file)
{
	fsync(fileno(file));

	if (unlock_flock(file))
		return 1;
	if (unlock_fcntl(file))
		return 2;
	return 0;
}

int lock_fcntl(FILE *file, char type)
{
	struct flock lock;
	if (type == 'w')
		lock.l_type = F_WRLCK;
	else if (type == 'r')
		lock.l_type = F_RDLCK;
	else
		return 1;

	lock.l_whence = SEEK_SET;
	lock.l_start = lock.l_len=0;

	if (fcntl(fileno(file), F_SETLKW, &lock) < 0)
	{
		return 1;
	}
	return 0;
}

int unlock_fcntl(FILE *file)
{
	struct flock lock;
	lock.l_type = F_UNLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = lock.l_len=0;

	if (fcntl(fileno(file), F_SETLKW, &lock) < 0)
	{
		return 1;
	}
	return 0;
}

int lock_flock(FILE *file)
{
	if (flock(fileno(file), LOCK_EX)<0)
		return 1;
	return 0;

}
int unlock_flock(FILE *file)
{
	if (flock(fileno(file), LOCK_UN)<0)
		return 1;
	return 0;
}
char * finddata(char *env, char *search, char answer[]) //finds data
{
	int i = 0;	//starts search at begining of env
	int notit = 0;
	int k = 0;
	while (env[i] != '\0')
	{
		k = 0; //Restart  search for search
		notit = 0;
		while (env[i] != '\0' && search[k]  != '\0')
		{
			//compares env and search
			if (env[i] == search[k])
			{
				k++;
				i++;
			}
			else //error, not it, loop around
			{
				notit = 1;
				i++;
				break;
			}
		}
		if (notit == 0) //found it, search to the end and copy data only
		{		//search key in not included?
			k = 0;
			while (env[i] != '\0' && env[i] != '&' && env[i] != ':' && env[i] != ';')
			{
				answer[k] = env[i];
				k++;
				i++;
			}
			answer[k] = '\0';
			return answer;
		}

	}
	return NULL;
}

//---------------------------------------------------
//public methods
void delete_all_messages(struct plugin_data *sysdata, int mailbox_num)
{
	//not implemented in the main program, nor in the display plugins.
	return;
}

//gets the message
int over_quota(struct plugin_data *sysdata, int mailbox_num, int size_bytes)
{

	struct stat size;
	char *path;

	path = convert(sysdata, mailbox_num);
	stat(path, &size);

	free(path); ///path freed!!!!!!!!!!!!!!!!

	if (size.st_size > size_bytes)
 	{
 		return 1;
 	}
	return 0;
}

struct mb_message* get_message(struct plugin_data *sysdata, int mailbox_num,
int message_num)
{
	char * path;
	struct mb_message *message; //needs to be on heap

	FILE *mailbox;
	char data[500];

	int count = 0;  //stores the message count
	int offofheader = 0; //mail header
	int subheader = 0; //multi line headers
	//int domarkold = 1;
	//int ismime = 0; //looks for mime header, only turns on for 1.0
	int onmimeheader = 0;

	char answer[500]; //stores found answer from boundary
	char boundary[300];

	//attachment stuff
	int dontdisplay = 0;


	boundary[0] = '-'; //sets up boundary
	boundary[1] = '-'; // "   "     "

	message = malloc(sizeof (struct mb_message));
	set_message_null(message); // clears the points to null


	path = convert(sysdata, mailbox_num); //find mailbox path
	if (path == NULL)
	{
		fprintf(stderr, "webemail:mb:local:get_message Bad mailbox discriptor or uid.\n");
		exit(1);
	}

	mailbox = fopen(path, "r");
	if (mailbox == NULL)
	{
		free(path);
		fprintf(stderr, "webemail:MB:local:get_message Error while tring to open mail file.\n");
		return NULL;
	}
	if (bothlock(mailbox, 'r')!=0)
	{
		free(path);
		fclose(mailbox);
		fprintf(stderr, "webemail:MB:local:get_message Error while locking mail file.\n");
		return NULL;
	}

	//gets message
	while (fgets(data, 499, mailbox) != NULL)
	{
		if (strncmp(data, "From ", 5)==0)
		{
			count++;
		}
		if (count > message_num)
			break;

		if (count == message_num)
		{
			if (data[0] == '\n')
				offofheader = 1;
			if (offofheader == 0) //in header
			{
				if (strncasecmp(data, "MIME-Version: 1.0",17)==0)
				{
					message->ismime=1;
				}
				else if (finddata(data, "boundary=", answer) != NULL)
				{
					int o = 0;
					if (answer[0] == '"')
						o++; //checks for"
					strncpy(boundary+2, answer+o, 300);
					for (o = 0; o<299; o++)
					{
						if (boundary[o] == '\n' ||
	 					boundary[o] == '"' ||
						boundary[o] == '\r' ||
						boundary[o] == '\0')
						{
							boundary[o] = '\0';
	 						break;
			 			}

					}
  				}
				//do extended data
 				else if (strncmp(data, " ",1)==0 || strncmp(data, "\t", 1) == 0)
				{
					subheader = extract_subheader_info(message, data, subheader);
				}
				else
				{
					subheader = extract_header_info(message, data);
				}
			}
			else //out of header
			{
				//turns on message display.
				if (message->ismime)
				{
					if (strncmp(data, boundary,strlen(boundary))==0)
 					{
						//found attachment header
						//allocates memory for attachment
						if (message->num_attach < 100)
						{
							onmimeheader = 1;
							dontdisplay = 0;
							message->num_attach++;
							message->attach[message->num_attach] = malloc(500);
							snprintf(message->attach[message->num_attach],
							499, "mime attachment %d", message->num_attach);
						}
		 			}
					if (onmimeheader && (
					finddata(data, "filename=", answer) != NULL))
					{
						strncpy(message->attach[message->num_attach], inquotes(answer), 489);
					}

					//check display type if text the display
					if (onmimeheader && (
					strncasecmp(data, "Content-Type: text/plain",24)==0 ||
					strncasecmp(data, "Content-Type: message/delivery-status",37)==0
					))
					{
						//turn on display, or
						//really leave on
						dontdisplay = 0;
					}
					else if (onmimeheader &&
					strncasecmp(data, "Content-Type: ", 14) == 0)
					{
						//unknown type, turn off display
						dontdisplay = 1;
					}
					else {}

				}
				else if (!message->ismime)
				{
				  	if (strncasecmp(data, "end", 3) ==0)
						dontdisplay = 0;
				}
				else {}

				//adds text to message
				if (dontdisplay == 0 && onmimeheader == 0) //ok to display
				{

					message->message = append_to(message->message, data);
				}
				else //on mime header or told not to display
				{
				}

				//turns off message display
				if (message->ismime)
				{
					if (onmimeheader && strncmp(data, "\n", 1) == 0)
					{
						onmimeheader = 0;
					}
				}
				else if (!message->ismime)
				{
					if (strncmp(data, "begin ", 6)==0)
					{
						dontdisplay = 1;
						if (message->num_attach < 100)
						{
							message->num_attach++;
							message->message = append_to(message->message,
							"Attached file removed from display.\n");
							message->attach[message->num_attach] = malloc(500);
							strncpy(message->attach[message->num_attach], data+10, 499);
						}
				    	}
				}
				else {}
			} // off header
		}//attach count == num
	} //while
	if (message->ismime && message->num_attach > 0)
	{
		//the closing boundry gets counted, mistake.
		free(message->attach[message->num_attach]);
		message->num_attach--;
	}

	bothunlock(mailbox);
	fclose(mailbox);
	free(path);
	return message;
}

//gets a list of messages
struct mb_list* get_message_list(struct plugin_data *plugindata, int mailbox_num)
{
	FILE *mailbox;
	char * path;
	char data[500];
	int count = 0;
	int onheader = 0;
	int hidden_count = 0;
	int size = 0;
	struct mb_list *list = NULL;
	struct mb_list *current = NULL;
	struct mb_message_header *message_head = NULL;

	path = convert(plugindata, mailbox_num);
	if (path == NULL)
	{
		fprintf(stderr, "webemail:mb:local:get_message_list Mailbox not found.\n");
		return NULL;
	}

	mailbox = fopen(path, "r");
	if (mailbox == NULL)
	{
		free(path);
		return NULL;
	}
	if (bothlock(mailbox, 'r')!=0)
	{
		free(path);
		fprintf(stderr, "webemail:mb:local:get_message_list Locking Error!!\n");
		return NULL;
	}


	while (fgets(data, 499, mailbox) != NULL)
	{
		if (strncmp(data, "From ", 5)==0)
		{
			count++;
			onheader = 1;
			//add old message to list
			if (message_head != NULL)
			{
				current = malloc(sizeof (struct mb_list));
				current->next = list;
				current->data = message_head;
				list = current;
			}

			message_head = malloc(sizeof (struct mb_message_header));
			//set default attributes
			message_head->to = NULL;
			message_head->from = NULL;
			message_head->date = NULL;
			message_head->subject = NULL;
			message_head->read = MB_NOT_READ;
			message_head->reply = MB_NOT_REPLIED;
			message_head->id_number = count;  //message id number
			message_head->hidden = MB_NOT_HIDDEN;	//system message not ment to be shown
		}

		if (data[0] == '\n' || data[0] == '\r')
		{
			onheader=0;
		}
		else if (onheader && strncasecmp(data, "From: ", 6)==0)
		{
			size = strlen(data+6)+1;
			message_head->from = malloc(size);
			strncpy(message_head->from, data+6, size);

			//check if message should be hidden
			if (count == 1 &&
			strncasecmp(message_head->from, "Mail System Internal Data", 25) == 0)
			{
				message_head->hidden = MB_HIDDEN;
				hidden_count++;
			}
		}
		else if (onheader && strncasecmp(data, "To: ", 4)==0)
		{
			size = strlen(data+4)+1;
			message_head->to = malloc(size);
			strncpy(message_head->to, data+4, size);
		}
		else if (onheader && strncasecmp(data, "Date: ", 6)==0)
		{
			size = strlen(data+6)+1;
			message_head->date = malloc(size);
			strncpy(message_head->date, data+6, size);
		}
		else if (onheader && strncasecmp(data, "Subject: ", 9)==0)
		{
			size = strlen(data+9)+1;
			message_head->subject = malloc(size);
			strncpy(message_head->subject, data+9, size);
		}
		else if (onheader && strncasecmp(data, "Status: ", 8)==0)
		{
			if (strncasecmp(data+8, "R", 1)==0)
				message_head->read = MB_READ;
		}
		else if (onheader && strncasecmp(data, "X-Status: ", 10)==0)
		{
			if (strncasecmp(data+10, "A", 1)==0)
				message_head->reply = MB_REPLIED;
		}
		else {}
	}

	//add last message to list
	if (message_head != NULL)
	{
		current = malloc(sizeof (struct mb_list));
		current->next = list;
		current->data = message_head;
		list = current;
		list->total_messages = count - hidden_count;
	}

	bothunlock(mailbox);
	fclose(mailbox);
	free(path);

	return list;
}

//gets the total number of messages
//returns -1 if system doesn't support or doesn't want to report
//total messages
int get_message_total(struct plugin_data *data, int mailbox)
{
	return -1;
}

//gets the attachment from the message.  //encoded or un???
//send data straight to a pipe.  don't want to store then write.
//that would requier a buffer.
// 0 = no error
// 1 = error
int get_attachment(struct plugin_data *plugindata, int mailbox_num, int message, int attachment,
FILE *pipe_out)
{
	FILE * mailbox;
	char * path;
	char data[500];
	int count = 0;
	int attachcount = 0;
	char header[500];
	int attachfound = 0;
	int onheader = 0;
	int ismime = 0;
	char contenttype[400];

	//mine stuff
	int extenddata = 0; //multi line headers
	int domarkold = 1;
	int onmimeheader = 0;
	int needsdecoding = 0;
	int size;
	int o;

	//decoder stuff
	int pipe[2];
	char decoder_path[500]; //not really needed
	char rdata[100];  //attach data
	char *arg[4];

	//fork stuff
	int fork_result;

	char answer[500];
	char boundary[300];
	boundary[0] = '-'; //sets up boundary
	boundary[1] = '-'; // "   "     "


	strncpy(contenttype, "text/plain\n", 399);

	path = convert(plugindata, mailbox_num);
	if (path == NULL)
	{
		fprintf(stderr, "webemail:mb:local:get_attachment Mailbox not found.\n");
		return 1;
	}

	mailbox = fopen(path, "r");
	if (mailbox == NULL)
	{
		free(path);
		fprintf(stderr, "webemail:mb:local:get_attachment can't open data for read.\n");
		return 1;
	}
	if (bothlock(mailbox, 'r')!=0)
	{
		free(path);
		fclose(mailbox);
		fprintf(stderr, "webemail:mb:local:get_attachment Error locking mailstore\n");
		return;
	}
//	info=getpwnam(globaluser);

	while (fgets(data, 500, mailbox) != NULL)
	{
		if (strncmp(data, "From ", 5)==0)
		{
			count++;
			onheader = 1;
			if (count > message)
				break;
		}
		if (count < message) //haven't found it yet, go back to the top.
			continue;

		//email found

		//off of header
		if (strncmp(data, "\n", 1) == 0)
		{
			onheader = 0;
		}
		//looking for Mime information
		if (onheader == 1 && strncmp(data, "MIME-Version: 1.0", 17) == 0)
		{
			ismime = 1;
		}
		if (onheader == 1)
		{
			if (finddata(data, "boundary=", answer) != NULL)
			{
				o = 0;
				if (answer[0] == '"')
					o++; //checks for"
				strncpy(boundary+2, answer+o, 298);
				for (o = 0; o<299; o++)
				{
					if (boundary[o] == '\n' ||
	 				boundary[o] == '"' ||
					boundary[o] == '\r' ||
					boundary[o] == '\0')
					{
						boundary[o] = '\0';
	 					break;
		 			}
                		}
  			}
		}

		if (onheader == 1)
			continue;  //still on header, need to go back

		//off of header
		//looking for start of uuencode block
		if (ismime == 0 && strncmp(data, "begin ", 6)==0)
		{
			attachcount++;
		}
		//look for start of mime block
		if (ismime == 1 && strncmp(data, boundary,strlen(boundary))==0)
 		{
			onmimeheader = 1;
			attachcount++;
		}

		if (attachcount < attachment)
			continue; //have not found the attachment yet, go back

		//found start of block
		//mime header information
		if (ismime == 1 && onmimeheader &&
		strncasecmp(data, "Content-Type: ", 14) == 0)
		{
			strncpy(contenttype, data+14, 399);
			removereturn(contenttype);
		}
		if (ismime == 1 && onmimeheader &&
		strncasecmp(data, "Content-Transfer-Encoding: base64",33)==0)
		{
			needsdecoding = 1;
		}
		//uuencode header information
		if (ismime == 0)
		{
			snprintf(contenttype, 500, "%s", guessmime(data));
		}
               	attachfound = 1;
		//send data through decoder
		if ((ismime == 1 && onmimeheader == 0) || ismime == 0)
		{
			//setup the decoder path
			if (ismime == 1 && needsdecoding)
			{
				snprintf(decoder_path, 500, "/usr/bin/mimencode");
				arg[0] = "mimencode";
				arg[1] = "-u";
				arg[2] = '\0';
			}
			else if (ismime == 0)
			{
				snprintf(decoder_path, 500, "/usr/bin/uudecode");
				arg[0] = "uudecode";
				arg[1] = "-o";
				arg[2] = "/dev/stdout";
				arg[3] = '\0';
			}
			else //mime but does not need decoding
			{
				snprintf(decoder_path, 500, "/bin/cat");
				arg[0] = "cat";
				arg[1] = '\0';
			}
			//do this in
			//make sure file handles are flushed to prevent duplicate
			//output from buffers.
			//fflush(stdout);
			//fflush(stdin);

			if (fullpipe_arg(decoder_path, arg, pipe))
			{
				free(path);
				fprintf(stderr,"webemail:mb:local:get_attachment Pipe error\n");
				return 1;
			}

			fork_result = fork();
			if (fork_result == (pid_t)-1) //error
			{
				free(path);
				fprintf(stderr, "webemail:mb:local:get_attachment fork error\n");
				return 1;
			}
			if (fork_result == (pid_t)0) //child writes
			{
				do
				{
					if (ismime == 1 && strncmp(data, boundary,strlen(boundary))==0)
					{
						break;
					}
					write(pipe[1], data, (size_t)(strlen(data)));
					if (ismime == 0 && strncmp(data, "end", 3)==0)
					{
						break;
					}
				} while (fgets(data, 500, mailbox) != NULL);
				close(pipe[1]);
				exit(0); //kill child
			}
			else //parent reads
			{
				close(pipe[1]); //prevents blocking
				attachcount++; //need to end
				fprintf(pipe_out, "content-type: %s", contenttype);
				fprintf(pipe_out, "\n\n");
				fflush(pipe_out);
				while ((size = read(pipe[0], rdata, 99))>0)
				{
					write(fileno(pipe_out), rdata, size);
				}
				close(pipe[0]);
				break;
			}
		}
		if (onmimeheader && strncmp(data, "\n", 1) == 0)
		{
			onmimeheader = 0;
		}
	}

	bothunlock(mailbox);
	fclose(mailbox);

	if (attachfound == 0)
	{
		free(path);
		fprintf(stderr, "webemail:mb:local:get_attachment No attachment found\n");
		return 1;
	}

	free(path);
}


//removes multible messages from the mail box.
void multi_delete_message(struct plugin_data *plugindata, int mailbox,
int to_del[], int array_size)
{
	struct passwd *info;
	FILE *data;
	FILE *tmp;
	char *path;

	char tmpname[400];
	char buf[500];
	int number = 0;
	int emailcount = 0;
	int delme = 0;
	int p = 0;

	path = convert(plugindata, mailbox);
	if (path == NULL)
	{
		fprintf(stderr, "webemail:mb:local:multi_delete_message Mailbox not found.\n");
		return;
	}

	info=getpwnam(plugindata->user_name);
	snprintf(tmpname, 400, "%s/.webmail/tmp", info->pw_dir);

	data = fopen(path, "r");
	if (data == NULL)
	{
		free(path);
		fprintf(stderr, "webemail:mb:local:multi_delete_message Error opening data for read.\n");
		return;
	}
	if (bothlock(data, 'r')!=0)
	{
		free(path);
		fclose(data);
		fprintf(stderr, "webemail:mb:local:multi_delete_message Error locking data for read\n");
		return;
	}
	tmp = fopen(tmpname, "w");
	if (tmp == NULL)
	{
		free(path);
		fprintf(stderr, "webemail:mb:local:multi_delete_message Error opening temp for write\n");
		fclose(data);
		return;
	}
	if (bothlock(tmp, 'w')!=0)
	{
		free(path);
		fclose(data);
		fclose(tmp);
		fprintf(stderr, "webemail:mb:local:multi_delete_message Error locking tmp for write\n");
		return;
	}
	while (fgets(buf, 499, data) != NULL)
	{
		fputs(buf, tmp);
	}
	bothunlock(data);
	fclose(data);
	bothunlock(tmp);
	fclose(tmp);

	sync();

	data = fopen(path, "w");
	if (data == NULL)
	{
		free(path);
		fprintf(stderr, "webemail:mb:local:multi_delete_message Error opening data for write\n");
		return;
	}
	if (bothlock(data, 'w')!=0)
	{
		free(path);
		fclose(data);
		fprintf(stderr, "webemail:mb:local:multi_delete_message Error locking data for write\n");
		return;
	}
	tmp = fopen(tmpname, "r");
	if (tmp == NULL)
	{
		free(path);
		fprintf(stderr, "webemail:mb:local:multi_delete_message Error opening temp for read\n");
		fclose(data);
		return;
	}
	if (bothlock(tmp, 'r')!=0)
	{
		free(path);
		fclose(data);
		fclose(tmp);
		fprintf(stderr, "webemail:mb:local:multi_delete_message Error locking temp for read\n");
		return;
	}

	while (fgets(buf, 499, tmp) != NULL)
	{
		if (strncmp(buf, "From ", 5)==0)
		{
			emailcount++;
			delme = 0;
			for (p = 0; p < array_size; p++)
			{
				if (emailcount == to_del[p])
				{
					delme = 1;
				}
			}
		}
		if (delme == 0)
		{
			fputs(buf, data);
		}
	}

	free(path);
	bothunlock(tmp);
	fclose(tmp);
	bothunlock(data);
	fclose(data);
	remove(tmpname);
	sync();

	return;

}

//removes a message from the mailbox
void delete_message(struct plugin_data *data, int mailbox, int message)
{
	int to_del[1];
	to_del[0] = message;
	multi_delete_message(data, mailbox, to_del, 1);
	return;
}


//copies a message from one mailbox to another
void copy_message(struct plugin_data *data, int src_mailbox, int dest_mailbox, int message)
{
	FILE *src;
	FILE *dest;
	char *src_path;
	char *dest_path;

	char buf[500];
	int count = 0;

	src_path = convert(data, src_mailbox);
	if (src_path == NULL)
	{
		fprintf(stderr, "webemail:mb:local:multi_delete_message Mailbox not found.\n");
		return;
	}
	dest_path = convert(data, dest_mailbox);
	if (dest_path == NULL)
	{
		free(src_path);
		fprintf(stderr, "webemail:mb:local:multi_delete_message Mailbox not found.\n");
		return;
	}

	src = fopen(src_path, "r");
	if (src == NULL)
	{
		free(src_path);
		free(dest_path);
		fprintf(stderr, "webemail:mb:local:multi_delete_message Error opening src for read.\n");
		return;
	}
	if (bothlock(src, 'r')!=0)
	{
		free(src_path);
		free(dest_path);
		fclose(src);
		fprintf(stderr, "webemail:mb:local:multi_delete_message Error locking src for read\n");
		return;
	}

	dest = fopen(dest_path, "a");
	if (dest == NULL)
	{
		free(src_path);
		free(dest_path);
		fprintf(stderr, "webemail:mb:local:multi_delete_message Error opening dest for write\n");
		fclose(src);
		return;
	}

	if (bothlock(dest, 'w')!=0)
	{
		free(src_path);
		free(dest_path);
		fclose(dest);
		fclose(src);
		fprintf(stderr, "webemail:mb:local:multi_delete_message Error locking dest for write\n");
		return;
	}
	while (fgets(buf, 499, src) != NULL)
	{
		if (strncmp(buf, "From ", 5)==0)
		{
			count++;
		}
		if (count == message)
		{
			fputs(buf, dest);
		}
		if (count > message)
		{
			break;
		}
	}

	free(src_path);
	free(dest_path);
	bothunlock(src);
	fclose(src);
	bothunlock(dest);
	fclose(dest);
	sync();

	return;
}

//movies a message from one mail box to another
void move_message(struct plugin_data *data, int src_mailbox, int dest_mailbox, int message)
{
	copy_message(data, src_mailbox, dest_mailbox, message);
	delete_message(data, src_mailbox, message);
	return;
}

//adds a message to a mail box.
void add_message(struct plugin_data *data, int mailbox, struct mb_message *message)
{
	FILE *dest;
	char *dest_path;

	char buf[500];
	int count = 0;

	dest_path = convert(data, mailbox);
	if (dest_path == NULL)
	{
		fprintf(stderr, "webemail:mb:local:multi_delete_message Mailbox not found.\n");
		return;
	}

	dest = fopen(dest_path, "a");
	if (dest == NULL)
	{
		free(dest_path);
		fprintf(stderr, "webemail:mb:local:multi_delete_message Error opening dest for write\n");
		return;
	}
	if (bothlock(dest, 'w')!=0)
	{
		free(dest_path);
		fclose(dest);
		fprintf(stderr, "webemail:mb:local:multi_delete_message Error locking dest for write\n");
		return;
	}

	fputs("\nFrom \n", dest);
	if (message->to != NULL)
	{
		removereturn(message->to);
		fputs("To: ", dest);
		fputs(message->to, dest);
		fputs("\n", dest);
	}
	if (message->from != NULL)
	{
		removereturn(message->from);
		fputs("From: ", dest);
		fputs(message->from, dest);
		fputs("\n", dest);
	}
	if (message->reply_to != NULL)
	{
		removereturn(message->reply_to);
		fputs("Reply-to: ", dest);
		fputs(message->reply_to, dest);
		fputs("\n", dest);
	}
	if (message->date != NULL)
	{
		removereturn(message->date);
		fputs("Date: ", dest);
		fputs(message->date, dest);
		fputs("\n", dest);
	}
	if (message->cc != NULL)
	{
		removereturn(message->cc);
		fputs("Cc: ", dest);
		fputs(message->cc, dest);
		fputs("\n", dest);
	}
	if (message->subject != NULL)
	{
		removereturn(message->subject);
		fputs("Subject: ", dest);
		fputs(message->subject, dest);
		fputs("\n", dest);
	}
	if (message->return_receipt_to != NULL)
	{
		removereturn(message->return_receipt_to);
		fputs("return-receipt-to: ", dest);
		fputs(message->return_receipt_to, dest);
		fputs("\n", dest);
	}
	if (message->read == MB_READ)
	{
		fputs("Status: RO\n", dest);
	}
	if (message->reply == MB_REPLIED)
	{
		fputs("X-Status: A\n", dest);
	}
	if (message->ismime == 1)
	{
		fputs("MIME-Version: 1.0\n", dest);
	}
	fputs("\n", dest);
	if (message->message != NULL)
		fputs(message->message, dest);

	free(dest_path);
	bothunlock(dest);
	fclose(dest);
	sync();

	return;
}

void mark(struct plugin_data *sysdata, int sys_mail_box, int message, int read, int reply)
{
	struct passwd *info;
	FILE *mailbox;
	FILE *tmpstore;
	char *path;
	char tmpname[400];
	char data[500];
	int count = 0;
	int readnotfound = 1;
	int replynotfound = 1;

	//get mail box settings
	info=getpwnam(sysdata->user_name);
	path = convert(sysdata, sys_mail_box); //find mailbox path
	if (path == NULL)
	{
		fprintf(stderr, "webemail:mb:local:mark Bad mailbox discriptor or uid.\n");
		exit(1);
	}
	snprintf(tmpname, 400, "%s/.webmail/tmp", info->pw_dir);

	//open mail boxes
	mailbox = fopen(path, "r");
	if (mailbox == NULL)
	{
		free(path);
		fprintf(stderr, "webemail:mb:local:mark Can't read mail box.\n");
		exit(1);
	}
	if (bothlock(mailbox, 'r')!=0)
	{
		free(path);
		fclose(mailbox);
		fprintf(stderr, "webemail:mb:local:mark Error locking mailstore\n");
		exit(1);
	}

	tmpstore = fopen(tmpname, "w");
	if (tmpstore == NULL)
	{
		free(path);
		fprintf(stderr, "webemail:mb:local:mark Error opening temp for write\n");
		fclose(mailbox);
		exit(1);
	}
	if (bothlock(tmpstore, 'w')!=0)
	{
		free(path);
		fclose(tmpstore);
		fclose(mailbox);
		error(stderr, "webemail:mb:local:mark Error locking tmpstore\n");
		exit(1);
	}
	while (fgets(data, 499, mailbox) != NULL)
	{
		fputs(data, tmpstore);
	}
	bothunlock(mailbox);
	fclose(mailbox);
	bothunlock(tmpstore);
	fclose(tmpstore);
	//make sure data has been written!!!!!!!
	sync();

	mailbox = fopen(path, "w");
	if (mailbox == NULL)
	{
		fprintf(stderr, "webemail:mb:local:mark Can't open mailbox for write\n");
		return; //errors will be encountered by calling function
	}
	if (bothlock(mailbox, 'w')!=0)
	{
		fclose(mailbox);
		fprintf(stderr, "webemail:mb:local:mark can't locking mailbox for write\n");
		return;
	}

	tmpstore = fopen(tmpname, "r");
	if (tmpstore == NULL)
	{
		return; //errors will be encountered by calling function
	}
	if (bothlock(tmpstore, 'r')!=0)
	{
		fclose(tmpstore);
		fclose(mailbox);
		fprintf(stderr, "webemail:mb:local:mark can't local temp for read.\n");
		return;
	}
	while (fgets(data, 499, tmpstore) != NULL)
	{
		if (strncmp(data, "From ", 5)==0)
		{
			count++;
		}
		if (count == message)
		{
			if (strncmp(data, "\n", 1)==0)
			{
				//gotten to end of header
				//with out finding the status
				//insert status
				count++;
				if (readnotfound == 1)
				{
					//mark old
					if (read == MB_READ)
					{
						fputs("Status: RO", mailbox);
						fputs("\n", mailbox);
					}
					else if (read == MB_NOT_READ)
					{
						fputs("Status: R", mailbox);
						fputs("\n", mailbox);
					}
					else
					{

					}
				}
				if (replynotfound == 1)
				{
					//mark old
					if (reply == MB_REPLIED)
					{
						fputs("X-Status: A", mailbox);
						fputs("\n", mailbox);
					}
					else if (reply == MB_NOT_REPLIED)
					{
						//not found, don't care, don't mark
					}
					else
					{

					}
				}
			}
			//status line is already there
			//so replace it
			if (strncasecmp(data, "Status: ", 8)==0)
			{
				if (read == MB_READ)
				{
					fputs("Status: RO", mailbox);
					fputs("\n", mailbox);
				}
				else if (read == MB_NOT_READ)
				{
					fputs("Status: R", mailbox);
					fputs("\n", mailbox);
				}
				else {}
				readnotfound = 0;
			}
			else if (strncasecmp(data, "X-Status: ", 10)==0)
			{
				if (reply == MB_REPLIED)
				{
					fputs("X-Status: A", mailbox);
					fputs("\n", mailbox);
				}
				else if (reply == MB_NOT_REPLIED)
				{
					//unmark replied, remove line
				}
				else {}
				replynotfound = 0;
			}
			else
				fputs(data, mailbox);


		}
		else
			fputs(data, mailbox);
	}
	bothunlock(tmpstore);
	fclose(tmpstore);
	bothunlock(mailbox);
	fclose(mailbox);
	remove(tmpname);
	sync();
	free(path);
	return;
}

