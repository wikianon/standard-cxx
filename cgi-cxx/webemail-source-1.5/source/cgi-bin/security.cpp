#include "webemail.h"
#include "security.h"
int refererOk()
{
	char * referer;

	char * host;
	char * request_uri;
	char cmp_url[500];

	referer = getenv("HTTP_REFERER");
	host = getenv("HTTP_HOST");
	request_uri = getenv("REQUEST_URI");

	if (referer == NULL || host == NULL || request_uri == NULL)
		return 2; //client does not support referer var
			  //client is allowed to go through

	if (inthere(referer, "/attachments/", 13) || 
	inthere(referer, "viewhtml=", 9))
		return 0;  //redirect from attachments  or html view, not good


	int i; //size for referer up to ?
		//any thing after ? is irrelivent
	for (i = 0; referer[i] != '?' && referer[i] != '\0'; i++)
	{
	} 	

	//check http responses
	snprintf(cmp_url, 499, "http://%s%s", host, request_uri);
	if (strncmp(cmp_url, referer, i) == 0)
		return 1; //client checks out ok.

	//check https responses
	snprintf(cmp_url, 499, "https://%s%s", host, request_uri);
	if (strncmp(cmp_url, referer, i) == 0)
		return 1; //client checks out ok.

	return 0; //problem found!!!!!
}
int encodeverify(char *user, char *pass) //checks uncoded pass and user
{
	passwd *temp;
	temp = getpwnam(user);
	if (temp == NULL)
	{
		return 1; //no user
	}
	decode(pass);
	if (verify(user, pass)==0)
	{
		return 0; //encrypt pass and user good
	}
	return 2; //bad pass
}
int verify(char *user, char *pass) //checks user and pass
{
	passwd *temp;
	char logerror[100];

	htmldecode(user);  //removes special html chars from user name
	htmldecode(pass);  //removes special html chars from password

	//block root access for security.
	if (ALLOWROOT == 0)
	{
		if (strcmp(user, "root") == 0)
		{
			error("Verify(): Root logins are not allowed");
			return 1; //no user
		}
	}
	#ifdef HAS_SHADOW
      	 spwd *spwddata;
  	#endif

	temp = getpwnam(user);
	if (temp == NULL)
	{
		openlog("webemail", LOG_NDELAY, LOG_AUTHPRIV);
		snprintf(logerror, 100, "User not found: %s", user);
		syslog(LOG_NOTICE | LOG_AUTHPRIV, logerror);
		closelog();
		strcpy(globaluser, "unknown user");
		return 1; //no user
	}

	#ifdef HAS_SHADOW
          spwddata = getspnam(user);
          if (spwddata != NULL)
             temp->pw_passwd = spwddata->sp_pwdp;
	  else
	  {
		strcpy(globaluser, "unknown user");
		error("Verify(): Error verifing shadow password");
		exit(0);
	  }
  	#endif

	 if (strcmp(crypt(pass, temp->pw_passwd),temp->pw_passwd)==0)
	 {
		strcpy(globaluser, temp->pw_name);
	 	return 0; //pass ok
	 }

	 openlog("webemail", LOG_NDELAY, LOG_AUTHPRIV);
	 snprintf(logerror, 100, "Wrong pass: %s", user);
	 syslog(LOG_NOTICE | LOG_AUTHPRIV, logerror);
	 closelog();

	 strcpy(globaluser, "unknown user");
	 return 2; //pass wrong
}

int setcookies(char line[400]) //sets user and pass cookies (encoded)
{
	char answer[400];
	char userspace[400]; //stores origial data 
	char passspace[400]; // then un htmled data
	//this data is not sent in the cookie
	char *user; 
	char *pass;

	//This data is sent back as the cookie
	char passspacehtml[400]; //save htmled pass
	char userspacehtml[400]; //  "    "


	passwd *temp;
	//find data
	user = finddata(400, line, "user=", userspace);
	pass = finddata(400, line, "pass=", passspace);	

	//copy htmled pass for submittion as cookie
	//if user and pass is correct
	strncpy(passspacehtml, pass, 400);
	strncpy(userspacehtml, user, 400);

	if (verify(user, pass)!=0) //distroys html encoding
	{
		return 0; //bad password
	}

	temp = getpwnam(user);
	if (temp == NULL)
	{
		return 0;
	}

	//user/pass ok, set cookies based on htmled pass
	cout << "Set-cookie: EUSER=" << userspacehtml << endl;
	encode(passspacehtml);
	cout << "Set-cookie: EPASS=" << passspacehtml << endl;
	cout << "Set-cookie: inbox_view=1-10" << endl; //set default view
	cout << "Set-cookie: saved_view=1-10" << endl; //set default view
	cout << "Set-cookie: sent_view=1-10" << endl; //set default view
	cout << "Set-cookie: current_view=index_inbox" << endl; //set default view

	//writes a log in /var/log/security saying they logged in
	openlog("webemail", LOG_NDELAY, LOG_AUTHPRIV);
	syslog(LOG_NOTICE | LOG_AUTHPRIV, globaluser);
	closelog();

	if (setgid(temp->pw_gid) !=0)
	{
		error("Setcookies(): can't switch group");
		exit(0);
	}
	if (setuid(temp->pw_uid) != 0)
	{
		error("Setcookies(): can't switch user");
		exit(0);
	}

	system("wesetup"); //lauch setup program
	return 1;
}
void encode(char *text) //encodes cookie text
{
	int i = 0;
	int temp;
	while (text[i] != '\0')
	{
		if (text[i] <= 'Z' && text[i] >= 'A')
		{
			temp = text[i];
			temp=temp + 10;
			if (temp > 'Z')
			{
				temp = temp-26;
			}
			text[i]=(char)temp;
		}
		if (text[i] <= 'z' && text[i] >= 'a')
		{
			temp = text[i];
			temp=temp + 10;
			if (temp > 'z')
			{
				temp = temp-26;
			}
			text[i]=(char)temp;
		}
		i++;
	}

}
void decode(char *text) //decodes cookie text
{
	int i = 0;
	while (text[i] != '\0')
	{
		if (text[i] <= 'Z' && text[i] >= 'A')
		{
			text[i] = text[i] - 10;
			if (text[i] < 'A')
				text[i] = text[i]+26;
		}
		if (text[i] <= 'z' && text[i] >= 'a')
		{
			text[i] = text[i] - 10;
			if (text[i] < 'a')
				text[i] = text[i]+26;

		}
		i++;
	}

}
int verifycookies() //checks cookie, user and pass
{
	char *env;
	char userspace[100];
	char passspace[100];
	char *user;
	char *pass;
	passwd *info;


	if ((env = getenv("HTTP_COOKIE")) != NULL)
	{

		user = finddata(100, env, "EUSER=", userspace);
		if (user == NULL)
		{
			cout << "Set-cookie: EUSER=none; expires=Wednesday, 09-JAN-1980 23:00:00 GMT" << endl;
			cout << "Set-cookie: EPASS=none; expires=Wednesday, 09-JAN-1980 23:00:00 GMT" << endl;
			return 1;
		}
		pass = finddata(100, env, "EPASS=", passspace);		
		if (pass == NULL)
		{
			cout << "Set-cookie: EUSER=none; expires=Wednesday, 09-JAN-1980 23:00:00 GMT" << endl;
			cout << "Set-cookie: EPASS=none; expires=Wednesday, 09-JAN-1980 23:00:00 GMT" << endl;
			return 1;
		}
		decode(pass);

		if (verify(user, pass)==0)
		{
			strcpy(globaluser, user);

			info=getpwnam(globaluser);
			if (setgid(info->pw_gid) !=0)
			{
				error("Verifycookies(): can't switch group");
				exit(1);
			}
			if (setuid(info->pw_uid) != 0)
			{
				error("verifycookies(): can't switch user");
				exit(1);
			}
			return 0; //user pass ok
		}
		return 1;
	}
	return 1;
}

