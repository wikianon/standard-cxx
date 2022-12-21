#ifndef DISPLUGIN_H
#define DISPLUGIN_H

struct config_data
{
	int show_pop;
	int nopop; //used if there is bad info in config file
	char *user;
	char *password;
	char *server;

	int show_forwarding;
	char *forwarding_address;

	int show_return;
	char *return_address;

	int show_signature;
	char *signature;
};

struct wewrite_data
{
	char *mode;
	// 0 normal
	// 1 reply
	// 2 from address book
	// 3 forward
	char *to;
	char *cc;
	char *subject;
	char *hidden;
};


#endif
