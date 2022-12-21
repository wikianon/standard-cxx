/*
	mail_process.h
*/

#ifndef _MAIL_PROCESS_H
#define _MAIL_PROCESS_H



int point_to_message(FILE* userfile,long int message_num);
			//will point to first line of message

char* point_to_attachment(FILE*,long int message_num,int att_num);
			//will point to first line of attachment in message
			//   returns ptr to boundary string, or NULL if err

int point_to_next_message(FILE*,ssize_t*);
			//reads stream until '\nFrom ' found

int update_status_read(FILE*,long int message_num);
			//will update message to indicate mail read

FILE* open_user_mailfile(char*);
			//opens user mail file with status char*

char* create_user_filename(void);
			//creates user filename

int insert_line_in_file(FILE*, char*, long int, long int);
			//inserts line in file

int file_exists(char*);
			//checks to see if file exists

char* eliminate_lwsp(char*);


#endif
