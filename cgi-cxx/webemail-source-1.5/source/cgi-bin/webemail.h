#ifndef _webemail_h
#define _webemail_h

#include "plugin.h"
#include "conf.h"
#include "displugin.h"
#include "mbplugin.h"
#include "addrplugin.h"
#include <stdio.h>

#define VER "1.5"
#define EMAILPATH "./email.cgi"
#define MAILBOXPATH "/var/spool/mail/%s", info->pw_name
//#define SENDMAIL "/usr/sbin/sendmail -t -i > /dev/null"

extern char *SENDMAIL;
extern int USEATTACHMENTS;
extern int HOSTMODE;
extern int FOUNDNAME;
extern char *HOSTNAME;
extern int USEQUOTAS;
extern int MAX_MBOX_SIZE;
extern int USEEXTERNMAIL;
extern int USELINKHOME;
extern char *LINKHOME;
extern int USELINKHOMENAME;
extern char *LINKHOMENAME;
extern char globaluser[400];
extern int USEISPHELP;
extern char *ISPHELP;
extern int USEAUTHORINFO;
extern int USERRETURN;
extern int ALLOWROOT;

extern void *display_plugin;
extern int (*get_interface_version)();
extern int (*get_plugin_subsystem)();
extern double (*get_plugin_version)();
extern char* (*get_language)();
extern char* (*get_plugin_name)();
extern void (*display_main_window)(plugin_data *, char *);
extern void (*display_main_title)(plugin_data *);
extern void (*display_main_menu)(plugin_data *);
extern void (*display_menu_head)(plugin_data *);
extern void (*display_menu_entree)(plugin_data *, char *, char *, char *, int);
extern void (*display_menu_tail)(plugin_data *, int, int, int, int);
extern void (*display_saved_menu_head)(plugin_data *);
extern void (*display_saved_menu_entree)(plugin_data *, char *, char *, char *, int);
extern void (*display_saved_menu_tail)(plugin_data *, int, int, int, int);
extern void (*display_sent_menu_head)(plugin_data *);
extern void (*display_sent_menu_entree)(plugin_data *, char *, char *, char *, int);
extern void (*display_sent_menu_tail)(plugin_data *, int, int,int, int);
extern void (*display_read_head)(plugin_data *, int);
extern void (*display_read_middle)(plugin_data *, int);
extern void (*display_read_attachments_head)(plugin_data *, int);
extern void (*display_read_attachments)(plugin_data *, int, int, char *, char *);
extern void (*display_read_attachments_tail)(plugin_data *, int);
extern void (*display_read_tail)(plugin_data*, int);
extern void (*display_saved_read_head)(plugin_data *, int);
extern void (*display_saved_read_middle)(plugin_data *, int);
extern void (*display_saved_read_attachments_head)(plugin_data *, int);
extern void (*display_saved_read_attachments)(plugin_data *, int, int, char *, char *);
extern void (*display_saved_read_attachments_tail)(plugin_data *, int);
extern void (*display_saved_read_tail)(plugin_data*, int);
extern void (*display_sent_read_head)(plugin_data *, int);
extern void (*display_sent_read_middle)(plugin_data *, int);
extern void (*display_sent_read_attachments_head)(plugin_data *, int);
extern void (*display_sent_read_attachments)(plugin_data *, int, int, char *, char *);
extern void (*display_sent_read_attachments_tail)(plugin_data *, int);
extern void (*display_sent_read_tail)(plugin_data*, int);
extern void (*display_write_head)(plugin_data *, wewrite_data *);
extern void (*display_write_tail)(plugin_data *, wewrite_data *);
extern void (*display_config)(plugin_data *, config_data *);
extern void (*display_about)(plugin_data *);
extern void (*display_login)(plugin_data *);
extern void (*display_error)(plugin_data *, char *);
extern void (*display_address_head)(plugin_data *);
extern void (*display_address_letterbreak)(plugin_data *, char);
extern void (*display_single_entre)(plugin_data *, addr_single_entry *);
extern void (*display_group_entre)(plugin_data *, addr_group_entry *);
extern void (*display_address_tail)(plugin_data *);
extern void (*display_address_add_group_head)(plugin_data *, addr_group_entry *, int);
extern void (*display_address_add_group_tail)(plugin_data *, addr_group_entry *, int);
extern void (*display_address_add_single)(plugin_data *, addr_single_entry *, int);
extern void (*display_view_html_head)(plugin_data *, int);
extern void (*display_saved_view_html_head)(plugin_data *, int);
extern void (*display_sent_view_html_head)(plugin_data *, int);
extern void (*display_view_html_tail)(plugin_data *, int);
extern void (*display_saved_view_html_tail)(plugin_data *, int);
extern void (*display_sent_view_html_tail)(plugin_data *, int);
extern void (*display_java_script_head)(plugin_data *);
extern void (*display_java_script_tail)(plugin_data *);

extern void *mb_plugin;
extern int (*mb_get_interface_version)();
extern int (*mb_get_plugin_subsystem)();
extern double (*mb_get_plugin_version)();
extern char* (*mb_get_plugin_name)();
extern void (*mb_free_mem)(void *data);
extern void (*mb_free_message)(mb_message * );
extern mb_message* (*mb_get_message)(plugin_data *, int, int);
extern void (*mb_mark)(plugin_data *, int, int, int, int);
extern mb_list* (*mb_get_message_list)(struct plugin_data *, int);
extern void (*mb_free_message_list)(struct mb_list*);
extern int (*mb_over_quota)(struct plugin_data *, int, int);
extern void (*mb_delete_message)(struct plugin_data *, int, int);
extern void (*mb_multi_delete_message)(struct plugin_data *, int, int *, int);
extern int (*mb_get_message_total)(struct plugin_data *, int);
extern void (*mb_copy_message)(struct plugin_data *, int, int, int);
extern void (*mb_move_message)(struct plugin_data *, int, int, int);
extern void (*mb_add_message)(struct plugin_data *, int, struct mb_message *);
extern int (*mb_get_attachment)(struct plugin_data *, int, int, int, FILE *);
extern void (*mb_delete_all_messages)(struct plugin_data *, int);
extern plugin_data *plugindata;

//address book
extern void *addr_plugin;
extern int (*addr_get_interface_version)();
extern int (*addr_get_plugin_subsystem)();
extern double (*addr_get_plugin_version)();
extern char* (*addr_get_plugin_name)();
extern void (*addr_free_mem)(void *);
extern void (*addr_free_addr_single_entry)(addr_single_entry *);
extern void (*addr_free_addr_group_entry)(addr_group_entry *);
extern void (*addr_free_addr_list)(addr_list *);
extern addr_list* (*addr_get_addr_list)(plugin_data *);
extern addr_group_entry* (*addr_get_group)(plugin_data *, int);
extern addr_single_entry* (*addr_get_single)(plugin_data *, int);
extern int (*addr_update_single_addr)(plugin_data *, addr_single_entry *);
extern int (*addr_update_group_addr)(plugin_data *, addr_group_entry *);
extern void (*addr_delete_single_addr)(plugin_data *, int);
extern void (*addr_delete_group_addr)(plugin_data *, int);

#ifdef HAS_SHADOW
  #include <shadow.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <crypt.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <sysexits.h>
#include <syslog.h>
#include <iostream>
#include <sys/stat.h>

#include <vector>

void intro();
void wewrite(int mode, char *email, int reply, int mailbox_num);
//reply=message to reply to -1 = don't
//-2 from address book
//now uses modes
int status(char line[400]);
char * finddata(int max_size, char *env, char search[], char answer[]);
void send();
void reset();
void menu();
void cfgstatus();
void error(char *mes);
void htmldecode(char *data);
void about();
void setup();
void removereturn(char * data);

void display_title();
void display_menu();
char * guessmime(char filename[]);

void quotestospace(char * data);
void commatospace(char * data);
void distroytag(char *data, char *tag);

int inthere(char * test1, char * test2, int num);
void scansend(char * search, char * data, int length, char * newboundary);
void display_conf();
void process_conf();
void disjavascript(char *url, char *message1, char *message2);
void escapeprint(char * data);
void popcheck();
char * cleanfilename(char *data);//removes " " ? \" ' from the file names so
				 //it does not interfear with the url line.
void doconfig();  //reads config file
void sendtext();
void sendmulti();
void handlesignals();


#endif

#include "bothlock.h"

