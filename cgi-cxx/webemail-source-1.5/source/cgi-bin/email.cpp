#include "plugin.h"
#include "webemail.h"
#include "save.h"
#include "sent.h"
#include "inbox.h"
#include "delete.h"
#include "security.h"
#include "pluginctl.h"
#include "displugin.h"
#include "mbplugin.h"
#include "address.h"
//ttd = things to do
//Use old send() for ie 3.0
//Fix foriegn chars in address and email, ie A with ' on top,
//ie chars above 128 to 256
//Change sent 'from' to sent 'to'.


//Global
char *SENDMAIL;
int USEATTACHMENTS;
int HOSTMODE;
int FOUNDNAME;
char *HOSTNAME;
int USEQUOTAS;
int MAX_MBOX_SIZE;
int USEEXTERNMAIL;
int USELINKHOME;
char *LINKHOME;
int USELINKHOMENAME;
char *LINKHOMENAME;
char globaluser[400];
int USEISPHELP;
char *ISPHELP;
int USEAUTHORINFO;
int USERRETURN;
int ALLOWROOT;

//plugin functions
void *display_plugin;

int (*get_interface_version)();
int (*get_plugin_subsystem)();
double (*get_plugin_version)();
char* (*get_language)();
char* (*get_plugin_name)();
void (*display_main_window)(plugin_data *, char *);
void (*display_main_title)(plugin_data *);
void (*display_main_menu)(plugin_data *);

void (*display_menu_head)(plugin_data *);
void (*display_menu_entree)(plugin_data *, char *, char *, char *, int);
void (*display_menu_tail)(plugin_data *, int, int, int, int);
void (*display_saved_menu_head)(plugin_data *);
void (*display_saved_menu_entree)(plugin_data *, char *, char *, char *, int);
void (*display_saved_menu_tail)(plugin_data *, int, int, int, int);
void (*display_sent_menu_head)(plugin_data *);
void (*display_sent_menu_entree)(plugin_data *, char *, char *, char *, int);
void (*display_sent_menu_tail)(plugin_data *, int, int, int, int);

void (*display_read_head)(plugin_data *, int);
void (*display_read_middle)(plugin_data *, int); 
void (*display_read_attachments_head)(plugin_data *, int);  
void (*display_read_attachments)(plugin_data *, int, int, char *, char *);  
void (*display_read_attachments_tail)(plugin_data *, int);  
void (*display_read_tail)(plugin_data*, int);

void (*display_saved_read_head)(plugin_data *, int);
void (*display_saved_read_middle)(plugin_data *, int); 
void (*display_saved_read_attachments_head)(plugin_data *, int);  
void (*display_saved_read_attachments)(plugin_data *, int, int, char *, char *);  
void (*display_saved_read_attachments_tail)(plugin_data *, int);  
void (*display_saved_read_tail)(plugin_data*, int);

void (*display_sent_read_head)(plugin_data *, int);
void (*display_sent_read_middle)(plugin_data *, int); 
void (*display_sent_read_attachments_head)(plugin_data *, int);  
void (*display_sent_read_attachments)(plugin_data *, int, int, char *, char *);  
void (*display_sent_read_attachments_tail)(plugin_data *, int);  
void (*display_sent_read_tail)(plugin_data*, int);

void (*display_write_head)(plugin_data *, wewrite_data *);  
void (*display_write_tail)(plugin_data *, wewrite_data *);
void (*display_config)(plugin_data *, config_data *);
void (*display_about)(plugin_data *);
void (*display_login)(plugin_data *);
void (*display_error)(plugin_data *, char *);
void (*display_address_head)(plugin_data *);
void (*display_address_letterbreak)(plugin_data *, char);
void (*display_single_entre)(plugin_data *, addr_single_entry *);
void (*display_group_entre)(plugin_data *, addr_group_entry *);
void (*display_address_tail)(plugin_data *);
void (*display_address_add_group_head)(plugin_data *, addr_group_entry *, int);
void (*display_address_add_group_tail)(plugin_data *, addr_group_entry *, int);
void (*display_address_add_single)(plugin_data *, addr_single_entry *, int);
void (*display_view_html_head)(plugin_data *, int);
void (*display_saved_view_html_head)(plugin_data *, int);
void (*display_sent_view_html_head)(plugin_data *, int);
void (*display_view_html_tail)(plugin_data *, int);
void (*display_saved_view_html_tail)(plugin_data *, int);
void (*display_sent_view_html_tail)(plugin_data *, int);

void (*display_java_script_head)(plugin_data *);
void (*display_java_script_tail)(plugin_data *);

//mail box plugin
void *mb_plugin;
int (*mb_get_interface_version)();
int (*mb_get_plugin_subsystem)();
double (*mb_get_plugin_version)();
char* (*mb_get_plugin_name)();
void (*mb_free_mem)(void *data);
void (*mb_free_message)(mb_message *);
mb_message* (*mb_get_message)(plugin_data *, int, int);
void (*mb_mark)(plugin_data *, int, int, int, int);
mb_list* (*mb_get_message_list)(plugin_data *, int);
void (*mb_free_message_list)(mb_list *);
int (*mb_over_quota)(struct plugin_data *, int, int);
void (*mb_delete_message)(struct plugin_data *, int, int);
void (*mb_multi_delete_message)(struct plugin_data *, int, int *, int);
int (*mb_get_message_total)(struct plugin_data *, int);
void (*mb_copy_message)(struct plugin_data *, int, int, int);
void (*mb_move_message)(struct plugin_data *, int, int, int);
void (*mb_add_message)(struct plugin_data *, int, struct mb_message *);
int (*mb_get_attachment)(struct plugin_data *, int, int, int, FILE *);
void (*mb_delete_all_messages)(struct plugin_data *, int);

//address book
void *addr_plugin;
int (*addr_get_interface_version)();
int (*addr_get_plugin_subsystem)();
double (*addr_get_plugin_version)();
char* (*addr_get_plugin_name)();
void (*addr_free_mem)(void *data);
void (*addr_free_addr_single_entry)(struct addr_single_entry *entry);
void (*addr_free_addr_group_entry)(struct addr_group_entry *entry);
void (*addr_free_addr_list)(struct addr_list *list);
struct addr_list* (*addr_get_addr_list)(struct plugin_data *plugindata);
struct addr_group_entry* (*addr_get_group)(struct plugin_data *plugindata, int uid);
struct addr_single_entry* (*addr_get_single)(struct plugin_data *plugindata, int uid);
int (*addr_update_single_addr)(struct plugin_data *plugindata, struct addr_single_entry *entry);
int (*addr_update_group_addr)(struct plugin_data *plugindata, struct addr_group_entry *entry);
void (*addr_delete_single_addr)(struct plugin_data *plugindata, int uid);
void (*addr_delete_group_addr)(struct plugin_data *plugindata, int uid);


plugin_data *plugindata;

main()
{
	plugindata = new plugin_data;
	plugindata->web_email_ver = new char[strlen(VER)+2];
	strncpy(plugindata->web_email_ver, VER, strlen(VER)+1); //plus one for null

	plugindata->web_email_support = new char[52];
	strncpy(plugindata->web_email_support, 
	"http://ashaw.dyn.cheapnet.net/webmail/support.html", 52);

	plugindata->url = new char[strlen(EMAILPATH)+2];
	strncpy(plugindata->url, EMAILPATH, strlen(EMAILPATH)+1);

	umask(066); //sets the mask
	handlesignals(); //prevent early interuption

	globaluser[0] = '\0';
	doconfig(); //sets up webemail

	setup_plugin();

	int todo;
	char line[400];
	todo = status(line);
	
FILE *lockfile;

	if (todo == 2) //no cookie but has pass and user name
	{
		if (setcookies(line) == 1)
			menu();
		else //cookies not set
			intro();
		plugindata->user_name = globaluser;
	}
	else if (todo == 4) //reset cookies and goto intro
	{
		reset();
		intro();
	}
	else if (verifycookies()==0) //user and password is valid
	{
		plugindata->user_name = globaluser;
		lockfile = processlock();

//		fprintf(stderr, "Starting webmail...");
//		fprintf(stderr, "%i ", todo);
//		char *f;
//		f = getenv("REQUEST_METHOD");
//		fprintf(stderr, "%s\n", f);
/************************************************
Attachments in web email could be exploted using
java script via a redirect to delete email and 
adresses or forward spam via a users account.  

refererOk fuction checks that the referer is
a valid web email page and not a page from an
attachment and will protect functions that could
be exploted.  referer only works with browser
that send the referer information.
*************************************************/
		if ((refererOk() == 0) &&
		(todo ==  8 || //delete
		todo == 34 || //multidel
		todo == 40 || //deletesaved
		todo == 41 || //savedmultidel
		todo == 44 || //deletesent
		todo == 45 || //sentmultidel
		todo == 32 || //procconf
		todo == 3 || //send
		todo == 13 || //addrdelete
		todo == 33 || //popcheck
		todo == 12 || //addproc
		todo == 16 || //addreproc
		todo == 19 || //gaddproc
		todo == 23 || //gaddreproc
		todo == 6)) //cfgstatus //for testing
		{
			disjavascript("", "Op denied", "main menu");
		}
		else if (todo == 1) //has cookie no other action
			menu();
		else if (todo == 3) //cookie and sending mail
			send();
		else if (todo == 5) //wants to write
			wewrite(0, "", 0, 0);
		else if (todo == 6) // display cfgstatus
			cfgstatus();
		else if (todo == 7) // read message
			read_inbox();
		else if (todo == 8)
			delete_inbox();
		else if (todo == 9)
			reply_inbox();
		else if (todo == 10)
			address();
		else if (todo == 11)
			add_single_addr("", -1);
		else if (todo == 12)
			process_single();
		else if (todo == 13)
			delete_addr();
		else if (todo == 14)
			s_sendto_addr();
		else if (todo == 15)
			edit_single_addr();
		else if (todo == 16)
			reprocess_single();
		else if (todo == 17)
			add_name_inbox();
		else if (todo == 18)
			add_group_addr(-1);
		else if (todo == 19)
			reprocess_group();
		else if (todo == 20)
			delete_group_addr();
		else if (todo == 21)
			g_sendto_addr();
		else if (todo == 22)
			edit_group_addr();
		else if (todo == 23)
			reprocess_group();
		else if (todo == 24)
			about();
		else if (todo == 25)
			setup();
		else if (todo == 26)
			viewhtml_inbox();
		else if (todo == 27)
			display_title();
		else if (todo == 28)
			display_menu();
		else if (todo == 29)
			index_inbox();
		else if (todo == 30)
		{	//prevents lock up on large files
			processunlock(lockfile);
			u_attach_inbox();
		}
		else if (todo == 31)
			display_conf();
		else if (todo == 32)
			process_conf();
		else if (todo == 33 && USEEXTERNMAIL)
		{	//needs to prevent a lock up
			//if popcheck gets stuck
			processunlock(lockfile);
			popcheck();
			return 0;
			//exit early so we don't unlock twice
		}
		else if (todo == 34)
			multi_del_inbox();
		else if (todo == 35)
			forward_inbox();
		else if (todo == 36)
		{
			viewprint_inbox();
		}
		else if (todo == 37)
			save_inbox();
		else if (todo == 38)
			index_saved();
		else if (todo == 39)
			read_saved();
		else if (todo == 40)
			delete_saved();
		else if (todo == 41)
			multi_del_saved();
		else if (todo == 42)
			index_sent();
		else if (todo == 43)
			read_sent();
		else if (todo == 44)
			delete_sent();
		else if (todo == 45)
			multi_del_sent();
		else if (todo == 46)
		{
			processunlock(lockfile);
			u_attach_saved();
		}
		else if (todo == 47)
		{
			processunlock(lockfile);
			u_attach_sent();
		}
		else if (todo == 48)
		{
			processunlock(lockfile);
			m_attach_inbox();
		}
		else if (todo == 49)
		{
			processunlock(lockfile);
			m_attach_saved();
		}
		else if (todo == 50)
		{
			processunlock(lockfile);
			m_attach_sent();
		}
		else if (todo == 51)
		{
			viewprint_sent();
		}
		else if (todo == 52)
		{
			viewprint_saved();
		}
		else if (todo == 53)
		{
			viewhtml_sent();
		}
		else if (todo == 54)
		{
			viewhtml_saved();
		}
		else if (todo == 55)
		{
			reply_saved();
		}
		else if (todo == 56)
		{
			forward_saved();
		}
		else //has no cookies or pass/user
			intro();

//		fprintf(stderr, "%i", todo);
//		fprintf(stderr, "....Stoping webmail\n\n");

		processunlock(lockfile);
	}
	else
		intro();

	close_plugin();

	return 0;
}
void reset() //sends two cookies to reset the user and password cookies
{
	cout << "Set-cookie: EUSER=none; expires=Wednesday, 09-JAN-1980 23:00:00 GMT" << endl;
	cout << "Set-cookie: EPASS=none; expires=Wednesday, 09-JAN-1980 23:00:00 GMT" << endl;

	return;
}
int status(char line[400]) //choices what should happen
{
	char *env;
	char userspace[100];
	char passspace[100];
	char *user;
	char *pass;

	if ((env = getenv("HTTP_COOKIE")) != NULL)
	{
		//check for problematic cookies
		user = finddata(100, env, "EUSER=", userspace);
		pass = finddata(100, env, "EPASS=", passspace);
		if (strncasecmp(userspace, "none", 4) == 0 ||
		user == NULL || pass == NULL)
		{
			if (fgets(line, 399, stdin)!=NULL)
			{
				line[399] = '\0';
				return 2; //getting pass from stdin
			}
			return 0; //intro screen
		}
		if ((env = getenv("QUERY_STRING")) != NULL)
		{
			if (strncmp(env, "send", 4) == 0)
			{
				return 3; //cookie & send mail
			}
			if (strncmp(env, "reset", 5) == 0)
			{
				return 4; //reset cookies
			}
			if (strncmp(env, "write", 5) == 0)
			{
				return 5; //write
			}
			if (strncmp(env, "cfgstatus", 9)==0)
			{
				return 6; //show cfgstatus
			}
			if (strncmp(env, "read_inbox=", 11)==0)
			{
				return 7; //wants to read an email message
			}
			if (strncmp(env, "delete_inbox=", 13)==0)
			{
				return 8; //delete a email message
			}
			if (strncmp(env, "reply_inbox=", 12)==0)
			{
				return 9; //reply to an email
			}
			if (strncmp(env, "book", 4)==0)
			{
				return 10; //goto addressbook
			}
			if (strncmp(env, "add_single_addr", 15)==0)
			{
				return 11; //add to addressbook
			}
			if (strncmp(env, "process_single", 14) == 0)
			{
				return 12; //proccess data to add to address book
			}
			if (strncmp(env, "delete_addr=", 12) == 0 )
			{
				return 13; //delete an address from the book
			}
			if (strncmp(env, "s_sendto_addr=", 14) == 0)
			{
				return 14; //send to selected user
			}
			if (strncmp(env, "edit_single_addr=", 17) == 0)
			{
				return 15; //edit an address
			}
			if (strncmp(env, "reprocess_single=", 17) == 0)
			{
				return 16; //add edited address
			}
			if (strncmp(env, "add_name_inbox=", 15) == 0)
			{
				return 17; //add new address from email
			}
			if (strncmp(env, "add_group_addr", 14) == 0)
			{
				return 18; //add group address
			}
			if (strncmp(env, "process_group", 13) == 0)
			{
				return 19; //proc address
			}
			if (strncmp(env, "delete_group_addr=", 18) == 0)
			{
				return 20; //remove group address
			}
			if (strncmp(env, "g_sendto_addr=", 14) == 0)
			{
				return 21; //send to group
			}
			if (strncmp(env, "edit_group_addr=", 16) == 0)
			{
				return 22; //edit group address
			}
			if (strncmp(env, "reprocess_group=", 16) == 0)
			{
				return 23; //readd group data
			}
			if (strncmp(env, "about", 5) == 0)
			{
				return 24; //about
			}
			if (strncmp(env, "wesetup", 7) == 0)
			{
				return 25; //run setup program
			}
			if (strncmp(env, "viewhtml_inbox=", 15) == 0)
			{
				return 26; //view e-mail in html mode
			}
			if (strncmp(env, "display_title", 13) == 0)
			{
				return 27; //view title
			}
			if (strncmp(env, "display_menu", 12) == 0)
			{
				return 28; //view menu
			}
			if (strncmp(env, "index_inbox", 11) == 0)
			{
				return 29; //view mail
			}
			if (strncmp(env, "u_attach_inbox=", 15) ==0)
			{
				return 30; //view attachment
			}
			if (strncmp(env, "conf", 4)==0)
			{
				return 31; //view config menu
			}
			if (strncmp(env, "process_conf", 12)==0)
			{
				return 32; //process conf
			}
			if (strncmp(env, "popcheck", 8)==0)
			{
				return 33; //check pop account
			}
			if (strncmp(env, "multi_del_inbox", 15)==0)
			{
				return 34; //multidel
			}
			if (strncmp(env, "forward_inbox=", 14)==0)
			{
				return 35; //forward
			}
			if (strncmp(env, "viewprint_inbox=", 16)==0)
			{
				return 36; //printable view
			}
			if (strncmp(env, "save_inbox=", 11)==0)
			{
				return 37;  //saves mail
			}
			if (strncmp(env, "index_saved", 11)==0)
			{
				return 38;
			}
			if (strncmp(env, "read_saved=", 11)==0)
			{
				return 39;
			}
			if (strncmp(env, "delete_saved=", 13)==0)
			{
				return 40;
			}
			if (strncmp(env, "multi_del_saved", 15)==0)
			{
				return 41;
			}
			if (strncmp(env, "index_sent", 10)==0)
			{
				return 42;
			}
			if (strncmp(env, "read_sent=", 10)==0)
			{
				return 43;
			}
			if (strncmp(env, "delete_sent=", 12)==0)
			{
				return 44;
			}
			if (strncmp(env, "multi_del_sent", 14)==0)
			{
				return 45;
			}
			if (strncmp(env, "u_attach_saved=", 15)==0)
			{
				return 46;
			}
			if (strncmp(env, "u_attach_sent=", 14)==0)
			{
				return 47;
			}
			if (strncmp(env, "m_attach_inbox=", 15)==0)
			{
				return 48;
			}
			if (strncmp(env, "m_attach_saved=", 15)==0)
			{
				return 49;
			}
			if (strncmp(env, "m_attach_sent=", 14)==0)
			{
				return 50;
			}
			if (strncmp(env, "viewprint_sent=", 15)==0)
			{
				return 51; //printable view
			}
			if (strncmp(env, "viewprint_saved=", 16)==0)
			{
				return 52; //printable view
			}
			if (strncmp(env, "viewhtml_sent=", 14)==0)
			{
				return 53; //printable view
			}
			if (strncmp(env, "viewhtml_saved=", 15)==0)
			{
				return 54; //printable view
			}
			if (strncmp(env, "reply_saved=", 12)==0)
			{
				return 55; //printable view
			}
			if (strncmp(env, "forward_saved=", 14)==0)
			{
				return 56; //printable view
			}

		}
		return 1; //cookie only
	}
	if (fgets(line, 399, stdin)!=NULL)
	{
		line[399] = '\0';
		return 2; //getting pass from stdin
	}

	return 0; //intro screen
}
