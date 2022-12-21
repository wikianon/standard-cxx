/*
	config.h

******NOTE******
If any of these parameters are changed, you should do a 'make clean'
****************

*/
		
#ifndef _CONFIG_H
#define _CONFIG_H


/*
	Typical Redhat-6.x locations
	httproot	"/home/httpd/html"
	cgibin		"/home/httpd/cgi-bin"

	Typical SuSE HTTP locations
	httproot	"/usr/local/httpd/htdocs"
	cgibin		"/usr/local/httpd/cgi-bin"

	Typical RedHat-7.0 HTTP locations
	httproot	"/var/www/html"
	cgibin		"/var/www/cgi-bin"

*/

#define HTTPROOT	"/home/httpd/html"
#define CGIBIN		"/home/httpd/cgi-bin"

#define	NMHTTP		"/nmail"


// If using the sendmail compatiability binary that comes with 
//    qmail or exim, uncomment the following
//    (gets rid of -U option when calling sendmail)
//#define SENDMAIL_N_U




#define SENDMAIL	"/usr/sbin/sendmail"
#define	MAILSPOOLDIR	"/var/spool/mail"


#define	LOCALSTATEDIR	"/var/nmail"
#define PREFDIR		"/var/nmail/pref"
#define LOCKDIR		"/var/nmail/lock"
#define TMPDIR		"/var/nmail/tmp"
#define SBINDIR		"/usr/sbin"

#define TMPPREFIX	"tmp_"
#define SESPREFIX	"ses_"

#endif
