/*
	view_message.h
*/

#ifndef _VIEW_MESSAGE_H
#define _VIEW_MESSAGE_H

#include "parse_mail_headers.h"
#include "main.h"


int view_message_action(void);
int view_attachment_action(void);


status_type find_next_border(FILE*, char*);
status_type is_boundary(char*, char*);


#endif
