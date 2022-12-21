/*
	encoding_conv.h
*/

#ifndef _ENCODING_CONV_H
#define _ENCODING_CONV_H


#ifndef _VIEW_MESSAGE_H
#include "view_message.h"
#endif


int text_to_html(char*);
int fix_quoted_to_text(char*);
status_type conv_and_out_base64_string(char*);

int conv_from_base64(int*, int*);


#endif
