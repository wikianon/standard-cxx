#include "webemail.h"
#include "attachments.h"

/************************************
finds data search in env and stores it in answer.
Stops at null, &, :, ;
Does not stop at carrage return. hopefull \n is at the end
Copies into answer.
Returns answer on found or null on falure
*************************************/
char * finddata(int max_size, char *env, char *search, char answer[]) //finds data
{
	int i = 0;	//starts search at begining of env
	int notit = 0;
	while (env[i] != '\0')
	{
		int k = 0; //Restart  search for search
		notit = 0;
		while (env[i] != '\0' && search[k]  != '\0')
		{
			//compares env and search
			if (env[i] == search[k])
			{
				k++;
				i++;
			}
			else //error, not it, loop around
			{
				notit = 1;
				i++;
				break;
			}
		}
		if (notit == 0) //found it, search to the end and copy data only
		{		//search key in not included?
			k = 0;
			while (env[i] != '\0' && env[i] != '&' && env[i] != ':' && env[i] != ';')
			{
				if (k >= max_size-1) //prevent buffer overflow attack
				{
					break;
				}
				answer[k] = env[i];
				k++;
				i++;
			}
			answer[k] = '\0';
			return answer;
		}

	}
	return NULL;
}
void htmldecode(char *data) //decode html
{
	int i = 0;
	int offset = 0;
	char value1;
	char value2;
	int intvalue = 0;

	while (data[offset] != '\0')
	{
		if (data[offset] == '+')
		{
			data[i] = ' ';
		}
		else if (data[offset] == '%')
		{
			value1 = data[offset+1];
			value2 = data[offset+2];

			if (value1 == '\0' || value2 == '\0')
				return;

			if (value1 <= '9' && value1 >= '0')
				intvalue = (value1-'0')*(int)pow(16, 1);
			else if (value1 <= 'f' && value1 >= 'a')
				intvalue =((value1-'a')+10)*(int)pow(16,1);
			else if (value1 <= 'F' && value1 >= 'A')
				intvalue = ((value1-'A')+10)*(int)pow(16,1);
			else
				intvalue = 0;

			if (value2 <= '9' && value2 >= '0')
				intvalue += (value2-'0');
			else if (value2 <= 'f' && value2 >= 'a')
				intvalue += ((value2-'a')+10);
			else if (value2 <= 'F' && value2 >= 'A')
				intvalue += ((value2-'A')+10);
			else 
				intvalue = 0;


			data[i] = (char)intvalue;
			offset+=2;
		}
		else 
		{
			data[i] = data[offset];
		}
		i++;
		offset++;
	}
	data[i] = '\0';
	return;
}


void setup()
{
	system("wesetup"); //lauch setup  program
	menu();
	return;
}

void removereturn(char * data)
{
	int org = 0;
	int Not = 0;
	while(data[org] != '\0')
	{
		if (data[org] == '\n')
			org++;
		data[Not] = data[org];
		if (data[org] == '\0')
			break;
		org++;
		Not++;
	}
	return;
}

char *  guessmime(char filename[])
{
	int i = 0;
	int ext = 0;
	while (filename[i] != '\0' && filename[i] != '\n')
	{
		if (filename[i] == '.')
		{
			ext = i;
		}
		i++;
	}

	if (strncasecmp(".jpeg", filename+ext, 5) == 0 ||
	strncasecmp(".jpg", filename+ext, 4) == 0 ||
	strncasecmp(".jpe", filename+ext, 4) == 0)
		return "image/jpeg";

	//defined at the bottom.
	//if (strncasecmp(".html", filename+ext, 5) == 0 ||
	//strncasecmp(".htm", filename+ext, 4) == 0)
	//	return "text/HTML";

	if (strncasecmp(".mp2", filename+ext, 4) == 0 ||
	strncasecmp(".mpa", filename+ext, 4) == 0 ||
	strncasecmp(".mpega", filename+ext, 6) == 0 ||
	strncasecmp(".mp3", filename+ext, 4) == 0)
		return "audio/x-mpeg";

	if (strncasecmp(".ra", filename+ext, 3) == 0 ||
	strncasecmp(".ram", filename+ext, 4)==0)
		return "audio/x-pn-realaudio-plugin";

	if (strncasecmp(".wpd", filename+ext, 4) == 0)
		return "application/wordperfect5.1";

	if (strncasecmp(".doc", filename+ext, 4) == 0)
		return "application/msword";

	if (strncasecmp(".js", filename+ext, 3) == 0)
		return "application/x-javascript";

	if (strncasecmp(".exe", filename+ext, 4) == 0)
		return "application/octet-stream";

	if (strncasecmp(".jar", filename+ext, 4) == 0)
		return "application/java-archive";

	if (strncasecmp(".avi", filename+ext, 4) == 0)
		return "video/x-msvideo";

	if (strncasecmp(".qt", filename+ext, 3) == 0 ||
	strncasecmp(".mov", filename+ext, 4)==0)
		return "video/quicktime";

	if (strncasecmp(".mpeg", filename+ext, 5) == 0 ||
	strncasecmp(".mpg", filename+ext, 4) == 0 ||
	strncasecmp(".mpe", filename+ext, 4) == 0 )
		return "video/mpeg";

	if (strncasecmp(".wav", filename+ext, 4) == 0)
		return "audio/x-wav";

	if (strncasecmp(".bmp", filename+ext, 4) == 0)
		return "image/x-ms-bmp";

	if (strncasecmp(".xbm", filename+ext, 4) == 0)
		return "image/x-xbitmap";

	if (strncasecmp(".tiff", filename+ext, 5) == 0 ||
	strncasecmp(".tif", filename+ext, 4) == 0)
		return "image/tiff";
	
	if (strncasecmp(".gif", filename+ext, 4) == 0)
		return "image/gif";

	if (strncasecmp(".rtf", filename+ext, 4) == 0)
		return "application/rtf";

	if (strncasecmp(".txt", filename+ext, 4) == 0 ||
	strncasecmp(".text", filename+ext, 5) == 0)
		return "text/plain";

	if (strncasecmp(".html", filename+ext, 5) == 0 ||
	strncasecmp(".htm", filename+ext, 4) == 0 ||
	strncasecmp(".shtml", filename+ext, 6) == 0)
		return "text/html";

	return "unknown/x-unknown";
}
void quotestospace(char * data)
{
	int i = 0;
	while (data[i] != '\0')
	{
		if (data[i] == '\"')
		{
			data[i] = ' ';
		}
		i++;
	}
	return;
}
void commatospace(char * data)
{
	int i = 0;
	while (data[i] != '\0')
	{
		if (data[i] == ',')
		{
			data[i] = ' ';
		}
		i++;
	}
	return;
}

/****************************************************
Checks if one string is contained in another.
String to check
Smaller string to compare
Length
I think
****************************************************/
int inthere(char * test1, char * test2, int num)
{
	int i1 = 0;
	int i2 = 0;
	int ok = 0;
	while (test1[i1] != '\0' && test2[i2] !='\0' && ok <= num)
	{
		if (test1[i1] == test2[i2])
			ok++;
		else
		{
			ok = 0;
			i2 = 0;
			i1++;
			continue;
		}
		if (ok == num)
			return 1;

		i1++;
		i2++;
	} 
	return 0;
}
void scansend(char * search, char * data, int length, char * newboundary)
{
	sprintf(search, "");
	while (fgets(data, length, stdin) != NULL)
	{
		if (strncasecmp(newboundary, data, strlen(newboundary)) == 0)
		{
			break;
		}
		else if (data[0] == '\n' || data[0] == '\r')
		{

		}
		else
			sprintf(search, "%s%s", search, data); 
			//end of data can have a \n, not sure this is ok?
	}

	return;
}
void process_conf()
{

	char *env;
	char *data;
	char *signature;
	char *forward;
	int length;
	char forwardpath[400];
	char signaturepath[400];
	FILE *forwardfile;
	FILE *signaturefile;

	char *popuser;
	char *poppass;
	char *popserver;
	char poppath[400];
	FILE *popfile;

	char *userreturn;
	char userreturnpath[400];
	FILE *userreturnfile;


	passwd *info;
	info=getpwnam(globaluser);
	if (info == NULL)
	{
		error("Procconf(): Can't get user info.");
		return;
	}

	if ((env = getenv("CONTENT_LENGTH"))==NULL)
	{
		error("Procconf(): Can't read environment.");
		return;
	}
	length = atoi(env);
	length+=5; // needed for null space

	data = new char[length];
	forward = new char[length];
	signature = new char[length];
	if (USERRETURN)
	{
		userreturn = new char[length];
	}
	if (USEEXTERNMAIL)
	{
		popuser = new char[length];
		poppass = new char[length];
		popserver = new char[length];
	}

	if (fgets(data, length, stdin) == NULL)
	{
		error("procconf(): Can't read form stdin.");
	}
	
	if (finddata(length, data, "sign=", signature) == NULL)
	{
		error("procconf(): Can't find sign tag.");
		return;
	}
	if (finddata(length, data, "forward=", forward) == NULL)
	{
		error ("procconf(): Can't find forward tag");
		return;
	}
	if (USERRETURN)
	{
		if (finddata(length, data, "userreturn=", userreturn) == NULL)
		{
			error ("procconf(): Can't find userreturn tag");
			return;
		}
	}
	if (USEEXTERNMAIL)
	{
		if (finddata(length, data, "popuser=", popuser) == NULL)
		{
			error("procconf(): Can't find popuser tag.");
			return;
		}
		if (finddata(length, data, "poppass=", poppass) == NULL)
		{
			error("procconf(): Can't find poppass tag.");
			return;
		}
		if (finddata(length, data, "popserver=", popserver) == NULL)
		{
			error ("procconf(): Can't find popserver tag");
			return;
		}
	}


	htmldecode(signature);
	snprintf(signaturepath, 400, "%s/.signature", info->pw_dir);
	if (signature[0] == '\0')
	{
		remove(signaturepath);
	}
	else
	{
		signaturefile = fopen(signaturepath, "w");
		if (signaturefile == NULL)
		{
			error("procconf(): can't open signature file for write.");
			return;
		}
		fputs(signature, signaturefile);
		fclose(signaturefile);
	}

	htmldecode(forward);
	snprintf(forwardpath, 400, "%s/.forward", info->pw_dir);
	if (forward[0] == '\0')
	{	
		remove(forwardpath);
	}
	else
	{
		forwardfile = fopen(forwardpath, "w");
		if (forwardfile == NULL)
		{
			error("procconf(): can't open forward file for write.");
			return;
		}
		if (bothlock(forwardfile, 'w')!=0)
		{
			fclose(forwardfile);
			error("procconf():Error locking tmpstore");
			return;		
		}
		fputs(forward, forwardfile);
		bothunlock(forwardfile);
		fclose(forwardfile);
	}

	if (USERRETURN)
	{
		htmldecode(userreturn);
		snprintf(userreturnpath, 400, "%s/.webmail/return",info->pw_dir);
		if (userreturn[0] == '\0')
		{
			remove(userreturnpath);
		}
		else
		{
			umask(066);
			userreturnfile = fopen(userreturnpath, "w");
			if (userreturnfile == NULL)
			{
				error("procconf(): can't open return file for write.");
				return;
			}
			if (bothlock(userreturnfile, 'w')!=0)
			{
				fclose(userreturnfile);
				error("procconf():Error locking return file");
				return;
			}
			//writes web e-mail's data
			fputs(userreturn, userreturnfile);
			bothunlock(userreturnfile);
			fclose(userreturnfile);
		}

	}
	if (USEEXTERNMAIL)
	{
		htmldecode(popuser);
		htmldecode(poppass);
		htmldecode(popserver);
		snprintf(poppath, 400, "%s/.fetchmailrc", info->pw_dir);
		if (popuser[0] == '\0')
		{	
			remove(poppath);
		}
		else
		{
			umask(066);
			popfile = fopen(poppath, "w");
			if (popfile == NULL)
			{
				error("procconf(): can't open fetchmail file for write.");
				return;
			}
			if (bothlock(popfile, 'w')!=0)
			{
				fclose(popfile);
				error("procconf():Error locking popfile");
				return;
			}
			//writes web e-mail's data
			fputs("# popuser=", popfile);
			fputs(popuser, popfile);
			fputs("&popserver=", popfile);
			fputs(popserver, popfile);
			fputs("&poppass=", popfile);
			fputs(poppass, popfile);
			fputs("\n\n", popfile);

			//writes warning
			fputs("# Configuration created by Web E-Mail.\n", popfile);
			fputs("# Changing this file will brake Web E-Mail's\n", popfile);
			fputs("# ablity to configure fetchmail.\n", popfile);
			fputs("# If you use Web e-mail's confuration utility\n", popfile);
			fputs("# after this file has been edited, you may lose\n", popfile);
			fputs("# your current edited fetchmail configuation.\n\n", popfile);
			
			//write fetchmail data
			fputs("set postmaster \"", popfile);
			fputs(globaluser, popfile);
			fputs("\"\n", popfile);

			fputs("set bouncemail\n", popfile);
			fputs("set properties \"\"\n", popfile);
			fputs("poll ", popfile); 
			fputs(popserver, popfile);
			fputs(" with proto POP3\n", popfile);

       			fputs("\tuser \"", popfile); 
			fputs(popuser, popfile);
			fputs("\" there with password \"", popfile);
			fputs(poppass, popfile);
			fputs("\" is ", popfile);
			fputs(globaluser, popfile);
			fputs(" here", popfile);

			bothunlock(popfile);
			fclose(popfile);
		}
	}

	delete data;
	delete forward;
	delete signature;
	if (USEEXTERNMAIL)
	{
		delete popuser;
		delete poppass;
		delete popserver;
	}
	sync();	
	disjavascript("", "System configured", "menu");

	return;
}
