#ifndef _security_h
#define _security_h

int refererOk(); //checks refering web page
int encodeverify(char *user, char *pass); //checks uncoded pass and user
int verify(char *user, char *pass); //checks user and pass
int setcookies(char line[400]); //sets user and pass cookies (encoded)
void encode(char *text); //encodes cookie text
void decode(char *text); //decodes cookie text
int verifycookies();

#ifdef OS_SunOS
 #ifndef LOG_AUTHPRIV
  #define LOG_AUTHPRIV LOG_AUTH
 #endif
#endif

#endif
