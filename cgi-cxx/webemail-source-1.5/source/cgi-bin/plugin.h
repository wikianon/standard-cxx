#ifndef PLUGIN_H
#define PLUGIN_H

//DO NOT WRITE TO ANY DATA MEMBER UNLESS IT SAYS IT IS OK TO DO
//SO!!!!

struct plugin_data
{
        char *user_name;        //users login id
        char *url;              //url to program, normally ./email.cgi
        char *server_name;      //only defined in httpd and define modes
        char *server_mode;      //httpd 1, define 2, sendmail 3
        char *isp_url;          //url to isp help page
        char *main_url;         //url to isp home page
        char *link_text;        //text to show on isp link to home page
        char *web_email_support; //url to web email support
        char *web_email_ver;    //web email version  

	//config settings
        int use_attachments;
        int use_externmail;
        int use_authorinfo;
        int userdefineablereturn;
 
        int use_isp_help;

        //arg for plugin. //version two only supports 2 args
	char *display_plugin;
        char *display_arg1;
        char *display_arg2;

	char *mb_plugin;
	char *mb_arg1;
	char *mb_arg2;

	char *addr_plugin;
	char *addr_arg1;
	char *addr_arg2;
};


#endif
