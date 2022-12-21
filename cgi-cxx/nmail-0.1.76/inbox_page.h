/*
	inbox_page.h
*/

#ifndef _INBOX_PAGE_H
#define _INBOX_PAGE_H

enum m_sort { 
   ms_date_a, 
   ms_date_d,
   ms_sender_a, 
   ms_sender_d,
   ms_size_a, 
   ms_size_d,
   ms_subj_a, 
   ms_subj_d,
   ms_none 
};

extern enum m_sort mail_sort;
int inbox_action(void);


#endif
