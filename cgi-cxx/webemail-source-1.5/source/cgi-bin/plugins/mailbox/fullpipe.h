/*************************************************************************
WARNING!!!!!!!!!!!!!!!!!!

Be very carefull when you use this functions.  It is very easy to block 
both the parent and the child causing both programs to lock up.  It is
best to have the parent fork again and do the reading and writing as
two processes.  That way one of the two parent processes will always not
be block and can clear up a block for the other two processes.
*************************************************************************/



#ifndef _fullpipe_h
#define _fullpipe_h
extern int fullpipe(char path[], int file_final[2]);
extern int fullpipe_arg(char path[], char *arg[], int file_final[2]);
#endif


