#include "webemail.h"
#include "fullpipe.h"
#include <time.h>

//not used, I don't think
void sendmulti()
{
	//sendmulti();
}
//not used, I don't think
void sendtext()
{
	char data[500];
	cout << "Content-type: plain/text" << endl;
	cout << "Pragma: no-cache" << endl << endl;

	while (fgets(data, 499, stdin)!=NULL)
	{
		cout << data;
	}
	return;
}
void send() //spawns sendmail to send mail
{
	passwd *info;
	char boundary[300];
	char newboundary[302];
	char *lengths;
 	int length = 0;
	char *data;
	char *env;
	int stage = 0;
	mb_message *message_to_add;

	char *to = NULL;
	char *cc = NULL;
	char *subject = NULL;
	char *message = NULL;
	char *file = NULL;   //file data to send
	char *filename = NULL;
	char *receipt = NULL;
	char *replied = NULL;
	char *mailbox = NULL;
	char *attachtype = NULL;
	int filesize = 0;
	char mimeboundary[37];  //36 chars plus null
	for (int l = 0; l < 37; l++)
		mimeboundary[l] = '-';
	mimeboundary[36] = '\0';

	char from[400];
	char reply_to[400];
	message_to_add = new mb_message;
	message_to_add->to = NULL;
	message_to_add->from = NULL;
	message_to_add->date = NULL;
	message_to_add->reply_to = NULL;
	message_to_add->cc = NULL;
	message_to_add->subject = NULL;
	message_to_add->return_receipt_to = NULL;
	message_to_add->read = MB_NOT_READ;
	message_to_add->reply = MB_NOT_REPLIED;
	message_to_add->ismime = 0;
	message_to_add->message = NULL;
	message_to_add->num_attach = 0;

	info=getpwnam(globaluser);
	if (info == NULL)
	{
		error("send(): Can't get user data");
		return;
	}

	env = getenv("CONTENT_TYPE");
	if (env == NULL)
	{
		error("send(): boundary data not found");
		return;
	}
	if(finddata(300, env, "boundary=", boundary)==NULL)
	{

		error("Send(): Can't find boundary");
		return;
	}
	snprintf(newboundary, 302, "--%s", boundary);

	lengths = getenv("CONTENT_LENGTH");
	if (lengths == NULL)
	{
		error("send(): Length data not found");
		return;
	}


	length = atoi(lengths);

	data = new char[length+10];
	fread(data, 1, length, stdin);

	int offset = 0;

	//get data from web browser
	while (offset < length)
	{
		if (strncmp("name=\"to\"", data+offset, 9) == 0 ||
		strncmp("name=to", data+offset, 7)==0)
		{
			while ((data[offset] != '\n' || (data[offset+1] != '\n' && data[offset+1] != '\r'))
			&& offset < length)
			{
				offset++;
			}

			offset+=3;  //takes care of the header separator
			to = data+offset;
			while (offset < (length-strlen(newboundary)))
			{
				if (strncmp(data+offset, newboundary, strlen(newboundary)) == 0)
				{
					data[offset-2] = '\0';
					break;
				}
				offset++;
			}
		}
		if (strncmp("name=\"cc\"", data+offset, 9) == 0 ||
		strncmp("name=cc", data+offset, 7) == 0)
		{
			while ((data[offset] != '\n' || (data[offset+1] != '\n' && data[offset+1] != '\r'))
			&& offset < length)
			{
				offset++;
			}

			offset+=3;  //takes care of the header separator
			cc = data+offset;
			while (offset < (length-strlen(newboundary)))
			{
				if (strncmp(data+offset, newboundary, strlen(newboundary)) == 0)
				{
					data[offset-2] = '\0';
					break;
				}
				offset++;
			}
		}
		if (strncmp("name=\"subject\"", data+offset, 14) == 0 ||
		strncmp("name=subject", data+offset, 12) == 0)
		{
			while ((data[offset] != '\n' || (data[offset+1] != '\n' && data[offset+1] != '\r'))
			&& offset < length)
			{
				offset++;
			}

			offset+=3;  //takes care of the header separator
			subject = data+offset;
			while (offset < (length-strlen(newboundary)))
			{
				if (strncmp(data+offset, newboundary, strlen(newboundary)) == 0)
				{
					data[offset-2] = '\0';
					break;
				}
				offset++;
			}
		}
		if (strncmp("name=\"message\"", data+offset, 14) == 0 ||
		strncmp("name=message", data+offset, 12) ==0)
		{
			while ((data[offset] != '\n' || (data[offset+1] != '\n' && data[offset+1] != '\r'))
			&& offset < length)
			{
				offset++;
			}

			offset+=3;  //takes care of the header separator
			message = data+offset;
			while (offset < (length-strlen(newboundary)))
			{
				if (strncmp(data+offset, newboundary, strlen(newboundary)) == 0)
				{
					data[offset-2] = '\0';
					break;
				}
				offset++;
			}
		}
		if (strncmp("filename=\"", data+offset, 10)==0 ||
		strncmp("filename=", data+offset, 10) == 0)
		{
//could have lots of problems here
			filename=data+offset+10;
			offset+=10;
			while (data[offset] != '"' && offset < length)
			{
				offset++;
			}
			data[offset] = '\0';

			while (!(data[offset] == 10 && data[offset-1] == 13 &&
			data[offset-2] == 10 && data[offset-3] == 13) &&
			offset < length)
			{
				offset++;
			}
			offset++;

			file = data+offset;
			while (offset < (length-strlen(newboundary)))
			{
				if (strncmp(data+offset, newboundary, strlen(newboundary)) == 0)
				{
					data[offset-2] = '\0';
					filesize = (data+offset-2)-file;
					break;
				}
				offset++;
			}
		}
		if (strncmp("name=\"receipt\"", data+offset, 14) == 0 ||
		strncmp("name=receipt", data+offset, 12) ==0)
		{
			while ((data[offset] != '\n' || (data[offset+1] != '\n' && data[offset+1] != '\r'))
			&& offset < length)
			{
				offset++;
			}

			offset+=3;  //takes care of the header separator
			receipt = data+offset;
			while (offset < (length-strlen(newboundary)))
			{
				if (strncmp(data+offset, newboundary, strlen(newboundary)) == 0)
				{
					data[offset-2] = '\0';  //sets null
					break;
				}
				offset++;
			}
		}
		if (strncmp("name=\"attachtype\"", data+offset, 17) == 0 ||
		strncmp("name=attachtype", data+offset, 15) == 0)
		{
			while ((data[offset] != '\n' || (data[offset+1] != '\n' && data[offset+1] != '\r'))
			&& offset < length)
			{
				offset++;
			}

			offset+=3;  //takes care of the header separator
			attachtype = data+offset;
			while (offset < (length-strlen(newboundary)))
			{
				if (strncmp(data+offset, newboundary, strlen(newboundary)) == 0)
				{
					data[offset-2] = '\0';  //sets null
					break;
				}
				offset++;
			}
		}
		if (strncmp("name=\"replied\"", data+offset, 14) == 0 ||
		strncmp("name=replied", data+offset, 12)== 0)
		{
			while ((data[offset] != '\n' || (data[offset+1] != '\n' && data[offset+1] != '\r'))
			&& offset < length)
			{
				offset++;
			}

			offset+=3;  //takes care of the header separator
			replied = data+offset;
			while (offset < (length-strlen(newboundary)))
			{
				if (strncmp(data+offset, newboundary, strlen(newboundary)) == 0)
				{
					data[offset-2] = '\0';  //sets null
					break;
				}
				offset++;
			}
		}
		if (strncmp("name=\"mailbox\"", data+offset, 14) == 0 ||
		strncmp("name=mailbox", data+offset, 12)== 0)
		{
			while ((data[offset] != '\n' || (data[offset+1] != '\n' && data[offset+1] != '\r'))
			&& offset < length)
			{
				offset++;
			}

			offset+=3;  //takes care of the header separator
			mailbox = data+offset;
			while (offset < (length-strlen(newboundary)))
			{
				if (strncmp(data+offset, newboundary, strlen(newboundary)) == 0)
				{
					data[offset-2] = '\0';  //sets null
					break;
				}
				offset++;
			}
		}

		offset++;
	}

	//write data to sendmail  and Set up message struct
	FILE * out;
	out = popen(SENDMAIL, "w");
	if (out == NULL)
	{
		error("Send(): Error opening or piping to sendmail");
		return;
	}

	//Split into uuencode and mime
	if (attachtype != NULL && strncmp("Mime", attachtype, 4)==0)
	{

		time_t mytime;
		mytime = time(NULL);
		srand((int)mytime);
		for (int l = 12; l<36; l++)
			mimeboundary[l] = ((int) (10.0*rand()/(RAND_MAX+1.0)))+'0';
	}


	//sending to
	if (to == NULL || to[0] == '\0')
	{
		error("Send(): \"To\" field is not filled in.");
		return;
	}
	removereturn(to);
	fprintf(out, "To: %s\n", to);
	message_to_add->to = to;

	//sending Return-Path
	FILE *returnfile;
	char returnpath[400];
	char returnaddr[400];
	snprintf(returnpath, 400, "%s/.webmail/return",info->pw_dir);
	returnfile = fopen(returnpath, "r");

	if (returnfile != NULL && USERRETURN)
	{
		for (int i = 0; i < 399; i++)
		{
			returnaddr[i] = fgetc(returnfile);
			if (returnaddr[i] == EOF)
			{
				returnaddr[i] = '\0';
				break;
			}
		}
		fclose(returnfile);
		fprintf(out, "Reply-to: <%s>\n", returnaddr);
		message_to_add->reply_to = returnaddr;
		fprintf(out, "From: %s\n", returnaddr);
		message_to_add->from = returnaddr;

		if (receipt && strncmp(receipt, "on", 2)==0)
			fprintf(out, "Return-Receipt-To: %s\n", returnaddr);
	}
	else
	{
		if (HOSTMODE == 2) //defined
		{
			fprintf(out, "Reply-to: <%s@%s>\n", info->pw_name, HOSTNAME);
			snprintf(reply_to, 400, "<%s@%s>\n", info->pw_name, HOSTNAME);
			message_to_add->reply_to = reply_to;

			fprintf(out, "From: %s@%s\n", info->pw_name, HOSTNAME);
			snprintf(from, 400, "%s@%s\n", info->pw_name, HOSTNAME);
			message_to_add->from = from;
			if (receipt && strncmp(receipt, "on", 2)==0)
				fprintf(out, "Return-Receipt-To: %s@%s\n", info->pw_name, HOSTNAME);
		}
		else if (HOSTMODE == 1)
		{
			char *host;
			if ((host = getenv("SERVER_NAME")) != NULL) //Get return path from server
			{
 				fprintf(out, "Reply-to: <%s@%s>\n", info->pw_name, host);
 				snprintf(reply_to, 400, "<%s@%s>\n", info->pw_name, host);
				message_to_add->reply_to = reply_to;

				fprintf(out, "From: %s@%s\n", info->pw_name, host);
				snprintf(from, 400, "%s@%s\n", info->pw_name, host);
				message_to_add->from = from;
				if (receipt && strncmp(receipt, "on", 2)==0)
					fprintf(out, "Return-Receipt-To: %s@%s\n", info->pw_name, host);

			}
			else //use sendmail if httpd fails
			{
				fprintf(out, "From: %s\n", info->pw_name);
				snprintf(from, 400, "%s\n", info->pw_name);
				message_to_add->from = from;
				if (receipt && strncmp(receipt, "on", 2)==0)
			 		fprintf(out, "Return-Receipt-To: %s\n", info->pw_name);
		 	}
		}
		else //Let sendmail fill in return path
		{
			fprintf(out, "From: %s\n", info->pw_name);
			snprintf(from, 400, "%s\n", info->pw_name);
			message_to_add->from = from;
			if (receipt && strncmp(receipt, "on", 2)==0)
		 		fprintf(out, "Return-Receipt-To: %s\n", info->pw_name);
		}
	}

	//sending X-Mailer
	fprintf(out, "X-Mailer: WEBEMAIL %s\n", VER);

	//sending cc
	if (cc[0] != '\0')
	{
		removereturn(cc);
		fprintf(out, "Cc: %s\n", cc);
		message_to_add->cc = cc;
	}

	//sending subject
	removereturn(subject);
	fprintf(out, "Subject: %s\n", subject);
	message_to_add->subject = subject;

	if (attachtype != NULL && strncmp("Mime", attachtype, 4)==0)
	{
		fprintf(out, "MIME-Version: 1.0\n");
		fprintf(out, "Content-Type: multipart/mixed;\n");
		fprintf(out, " boundary=\"%s\"\n", mimeboundary);
	}

	//sending header break
	fprintf(out, "\n");  //header break

	if (attachtype != NULL && strncmp("Mime", attachtype, 4)==0)
	{
		fprintf(out, "This is a MIME-encapsulated message.\n\n");
	}

	//sending message
	if (attachtype != NULL && strncmp("Mime", attachtype, 4)==0)
	{
		//start mime section
		fprintf(out, "--%s\n", mimeboundary);
		fprintf(out, "Content-Type: text/plain; charset=us-ascii\n");
		fprintf(out, "Content-Transfer-Encoding: 7bit\n");
		fprintf(out, "\n");
		fprintf(out, "%s", message);
		message_to_add->message = message;

		fprintf(out, "\n\n");
	}
	else
	{
		fprintf(out, "%s", message);
		message_to_add->message = message;
	}


	if (filesize > 0)
	{
		int usebase64 = 0;

		if (attachtype != NULL && strncmp("Mime", attachtype, 4)==0)
		{
			fprintf(out, "--%s\n", mimeboundary);
			fprintf(out, "Content-Type: ");
			fprintf(out, "%s\n", guessmime(filename));

			//decide on how to encode data
			if (strcmp(guessmime(filename), "text/html") == 0 ||
			strcmp(guessmime(filename), "text/plain") == 0)
			{
				fprintf(out, "Content-Transfer-Encoding: 7bit\n");
				usebase64 = 0;
			}
			else
			{
				fprintf(out, "Content-Transfer-Encoding: base64\n");
				usebase64 = 1;
			}
			fprintf(out, "Content-Disposition: inline,\n");
			fprintf(out, " filename=\"%s\"\n",cleanfilename(filename));

			fprintf(out, "\n");
		}

		char encodepath[500];
		char buf;
		int pipe[2];
		char *arg[3];

		if (attachtype != NULL && strncmp("Mime", attachtype, 4)==0 && usebase64)
		{
			snprintf(encodepath, 500, "/usr/bin/mimencode");
			arg[0] = "mimencode";
			arg[1] = '\0';
		}
		else if (attachtype != NULL && strncmp("Mime", attachtype, 4)==0 && usebase64 == 0)
		{
			snprintf(encodepath, 500, "/bin/cat");
			arg[0] = "cat";
			arg[1] = '\0';
		}
		else
		{
			snprintf(encodepath, 500, "/usr/bin/uuencode");
			arg[0] = "uuencode";
			arg[1] = filename;
			arg[2] = '\0';
		}

		//make sure any child process don't have data in the buffer
		//that might be writen on their termination.
		fflush(out);
		fflush(stdout);
		fflush(stdin);

		if (fullpipe_arg(encodepath, arg, pipe))
		{
			error("send(): Problem with pipe");
			return;
		}
		//fork to prevent blocking
		int forkresult;
		forkresult = fork();
		if (forkresult == (pid_t)-1)
		{
			error("send(): Problem forking");
			return;
		}
		if (forkresult == (pid_t)0) //child
		{
			write(pipe[1], file, filesize);
			close(pipe[1]);
			exit(0);
		}
		else //parent
		{
			close(pipe[1]); //prevents block on open write

			if (attachtype != NULL && strncmp("Mime", attachtype, 4)==0)
			{}
			else
			{
				fputs("\n\n", out);
			}

			while (read(pipe[0], &buf, 1))
			{
				fputc(buf, out);
			}
			close(pipe[0]);

			if (attachtype != NULL && strncmp("Mime", attachtype, 4)==0)
			{
				fprintf(out, "\n");
			}
		}
	}

	if (attachtype != NULL && strncmp("Mime", attachtype, 4)==0)
	{
		fprintf(out, "--%s--\n", mimeboundary);
	}

	//MARK REPLIED BEFORE LETTING SENDMAIL SEND
	//OR SENDMAILS MESSAGE MAY BE OVER WRITTEN
	int repliednum = atoi(replied);
	int mailboxnum = 0;
	if (mailbox != NULL)
		mailboxnum = atoi(mailbox);
	if (repliednum > 0)
	{
		(*mb_mark)(plugindata, mailboxnum, repliednum, MB_NO_OP, MB_REPLIED);
	}

	//displays sendmail return errors
	if (pclose(out) != 0)
	{
		error("sendmail: bad local \"to\"/\"cc\" address or internal sendmail problem.");
		return;
	}

	(*mb_add_message)(plugindata, MB_SENT, message_to_add);
	delete message_to_add;

	disjavascript("", "Mail sent", "menu");
	delete(data);

}

