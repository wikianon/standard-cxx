#include "webemail.h"

void cfgstatus() //displays basic configuration info
{
	passwd *info;

	cout << "content-type: text/plain" << endl;
	cout << "Pragma: no-cache" << endl << endl;

	cout << "----------------------" << endl;
	cout << "Web Mail Config Status" << endl;
	cout << "----------------------" << endl;
	cout << endl;
	cout << "Version: " << VER << endl;
	cout << "Executable name: " << EMAILPATH << endl;
	cout << endl;

	cout << "User ID: ";
	cout << getuid() << endl;
	info=getpwuid(getuid());
	cout << "(" << info->pw_name << ")" << endl;
	cout << "User Group: ";
	cout << getgid() << endl << endl;

	cout << "Effective user id: ";
	cout << geteuid() << endl;
	info=getpwuid(geteuid());
	cout << "(" << info->pw_name << ")" << endl;
	cout << "Effective Group: ";
	cout << getgid() << endl;

	cout << endl;
	cout << "Plugin subsystems:" << endl;
	cout << "------------------------------" << endl;
	cout << " Display plugin information." << endl;
	cout << "  Name: " << (*get_plugin_name)() << endl;
	cout << "  Version: " << (double)(*get_plugin_version)() << endl;
	cout << "  Language: " << (*get_language)() << endl;
	cout << " Mailbox plugin information." << endl;
	cout << "  Name: " << (*mb_get_plugin_name)() << endl;
	cout << "  Version: " << (double)(*mb_get_plugin_version)() << endl;
	cout << " Address plugin information." << endl;
	cout << "  Name: " << (*addr_get_plugin_name)() << endl;
	cout << "  Version: " << (double)(*addr_get_plugin_version)() << endl;
	cout << endl;

	cout << "#Defines (settings)" << endl;
	cout << "------------------------------" << endl;
	#ifdef HAS_SHADOW
	  cout << "Using shadow passwords." << endl;
	#endif

	if (USEATTACHMENTS)
	  cout << "Using attachments." << endl;

	if (USEQUOTAS)
	  cout << "Using mail box quota: " << MAX_MBOX_SIZE << endl;

	if (USEEXTERNMAIL)
	  cout << "Using Fetchmail." << endl;

	cout << "SendMailPath: " << SENDMAIL << endl;

	cout << endl;
	cout << "Copyright (c) 1999-2001 Andrew Shaw, All rights reserved.";

	return;
}
void error(char *mes) //error messages
{

	cout << "Content-type: text/HTML" << endl;
	cout << "Pragma: no-cache" << endl << endl;

	(*display_error)(plugindata, mes);

	exit(1);
	return;
}
void about()
{
	cout << "Content-type: text/HTML" << endl;
	cout << "Pragma: no-cache" << endl << endl;
	(*display_about)(plugindata);

	return;
}

//busts up html tags to prevent js attacks
void distroytag(char *data, char *tag)
{
	int i = 0;
	while (data[i] != '\0')
	{
		if (strncasecmp(data+i, tag, strlen(tag)) == 0)
			data[i] = '*';
		i++;
	}

}
