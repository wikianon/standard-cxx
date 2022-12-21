/*
	parse_mail_headers.h
*/

#ifndef _PARSE_MAIL_HEADERS_H
#define _PARSE_MAIL_HEADERS_H

#include <time.h>
#include "addr.h"

enum mail_s { m_read, m_new };

enum content_type {
   ct_text_plain,
   ct_text_html,
   ct_text_other,

   ct_multipart_mixed,
   ct_multipart_alternative,
   ct_multipart_parallel,
   ct_multipart_digest,
   ct_multipart_other,

   ct_message_rfc822,
   ct_message_partial,
   ct_message_externalbody,
   ct_message_other,

   ct_application_pdf,
   ct_application_zip,
   ct_application_msword,
   ct_application_msexcel,
   ct_application_other,

   ct_image_jpeg,
   ct_image_gif,
   ct_image_png,
   ct_image_other,

   ct_audio_other,
   ct_video_other,

   ct_other,
   ct_none

};

enum content_type_encoding {
   cte_7bit,			// \ 
   cte_8bit,			//  - no encoding
   cte_binary,			// /
   cte_quoted_printable,
   cte_base64,
   cte_none
};

enum content_disposition {
   ctd_inline,
   ctd_attachment,
   ctd_none
};

struct mail_header {
   int m_num;
   struct mail_header* next_header;
   ssize_t size_of_message;

   enum mail_s mail_status;
   time_t date_received;

   addr_t* from;
   addr_t* to;
   addr_t* cc;
   addr_t* reply_to;
   char* subject;

   content_type cont_type;
   char* cont_type_string;
   char* cont_boundary;
   content_type_encoding cont_type_enc;
   content_disposition cont_disposition;
   char* filename;
};


struct mail_header* fill_mail_h_struct(FILE*, int);
void free_mail_headers(struct mail_header*);

struct mail_header* get_mail_headers(void);
int sort_mh(struct mail_header*,struct mail_header***);
int init_mail_file_header(FILE*);

content_type determine_content_type(char*);
content_type_encoding determine_content_type_enc(char*);



#endif
