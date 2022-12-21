#include "webemail.h"
#include "delete.h"
#include <ctype.h>

void address() //displays address book
{
	struct addr_list* list;
	struct addr_list* current;

	list = (*addr_get_addr_list)(plugindata);
// 	if (list == NULL)
// 	{
// 		cout << "Content-type: text/html" << endl;
// 		cout << "Pragma: No-Cache" << endl << endl;
//
// 		cout << "<font size=\"+3\">Error opening file!</font><br>";
// 		cout << "Check the suid settings and user directory.<br>";
// 		cout << "To create user directory and other files use <a href=\"";
// 		cout << plugindata->url << "?wesetup\" TARGET=_top>";
// 		cout << "wesetup</a>.";
// 		return;
// 	}
	current = list;

	cout << "Content-type: text/HTML" << endl;
	cout << "Pragma: No-Cache" << endl << endl;

	(*display_address_head)(plugindata);

	char inc = 'A';
	char letter;

	(*display_address_letterbreak)(plugindata, inc);

	while (current != NULL)
	{
		//find current letter
		if (current->type == ADDR_SINGLE)
		{
			letter = ((addr_single_entry*)(current->entry))->name[0];
		}
		else if (current->type == ADDR_GROUP)
		{
			letter = ((addr_group_entry*)(current->entry))->name[0];
		}
		else
		{

		}
		//update the index
		letter = toupper(letter);
		if (inc < letter)
		{
			while (inc < letter && inc <= 'Z' && inc >= 'A')
			{
				inc++;
				(*display_address_letterbreak)(plugindata, inc);
			}
		}

		//display entries
		if (current->type == ADDR_SINGLE)
		{

			(*display_single_entre)(plugindata,
			(addr_single_entry*)(current->entry));
		}
		else if (current->type == ADDR_GROUP)
		{
			(*display_group_entre)(plugindata,
			(addr_group_entry*)(current->entry));
		}
		else
		{

		}
	 	current = current->next;
	}
	//make sure we get last index
	letter = 'Z';
	while (inc < letter && inc <= 'Z' && inc >= 'A')
	{
		inc++;
		(*display_address_letterbreak)(plugindata, inc);
	}

	(*addr_free_addr_list)(list);
	(*display_address_tail)(plugindata);

	return;
}
void add_single_addr(char *fill, int number) //fill is email to fill space
{
	int addedit;

	addr_single_entry *addressinfo;

	if (number > 0) //edit one in address book
	{
		//need to get the info from the data base
		addressinfo = (*addr_get_single)(plugindata, number);
		if (addressinfo == NULL)
		{
			error("add_single_addr():Error reading address book.");
			return;
		}
	}
	else
	{
		addressinfo = new addr_single_entry;
		addressinfo->name = "";
		if (number == -2)
		{
			quotestospace(fill);
			commatospace(fill);
			addressinfo->email_address = fill;
		}
		else
		{
			addressinfo->email_address = "";
		}
		addressinfo->phone = "";
		addressinfo->address_line_1 = "";
		addressinfo->fax = "";
		addressinfo->address_line_2 = "";
		addressinfo->notes = "";

	}

	cout << "Content-type: text/HTML" << endl;
	cout << "Pragma: No-Cache" << endl << endl;


	if (number > 0)  //edit
		addedit = 1;
	else //add
		addedit = 0;

	addressinfo->uid = number;

	(*display_address_add_single)(plugindata, addressinfo, addedit);
	if (number > 0)
	{
		(*addr_free_addr_single_entry)(addressinfo);
	}
	else
	{
		delete addressinfo;
	}

	return;
}
void process_single() //process data for addressbook
{
	char data[4000];

	char name[100];
	char *pname;
	char fax[100];
	char *pfax;
	char email[100];
	char *pemail;
	char phone[100];
	char *pphone;
	char address1[100];
	char *paddress1;
	char address2[100];
	char *paddress2;
	char notes[200];
	char *pnotes;
	addr_single_entry *entry;
	entry = new addr_single_entry;


	if (fgets(data, 3999, stdin)==NULL)
	{
		error("process_single(): Processing error.");
		return;
	}
	pname = finddata(100, data, "name=", name);
	htmldecode(name);
	pemail = finddata(100, data, "email=", email);
	htmldecode(email);
	pfax = finddata(100, data, "fax=", fax);
	htmldecode(fax);
	pphone = finddata(100, data, "phone=", phone);
	htmldecode(phone);
	paddress1 = finddata(100, data, "addressline1=", address1);
	htmldecode(address1);
	paddress2 = finddata(100, data, "addressline2=", address2);
	htmldecode(address2);
	pnotes = finddata(200, data, "notes=", notes);
	htmldecode(notes);

	entry->name = pname;
	entry->email_address = email;
	entry->fax = fax;
	entry->phone = phone;
	entry->address_line_1 = address1;
	entry->address_line_2 = address2;
	entry->notes = notes;
	entry->uid = ADDR_UID_ADD;

	//incoming name is not a letter, do not allow it as a name
	if (isalpha(name[0])==0)
	{
		address();
		return;
	}

	if ((*addr_update_single_addr)(plugindata, entry))
	{
		error("process_single(): Error with address book.");
		return;
	}
	//don't need to delete the other vars because it is on
	//the stack not the heap.
	delete entry;

	disjavascript("?book", "Entree entered", "address book");

	return;
}
void edit_single_addr()
{
	char *env;
	int number = 0;

	if ((env = getenv("QUERY_STRING")) == NULL)
	{
		error("edit_single_addr(): Environment error.");
		return;
	}

	number = atoi(env+17);

	add_single_addr("", number);

	return;
}
void reprocess_single()
{
	char *env;
	int number = 0;
	if ((env = getenv("QUERY_STRING")) == NULL)
	{
		error("reprocess_single(): Environment error.");
		return;
	}
	number = atoi(env+17);


	char data[4000];

	char name[100];
	char *pname;
	char fax[100];
	char *pfax;
	char email[100];
	char *pemail;
	char phone[100];
	char *pphone;
	char address1[100];
	char *paddress1;
	char address2[100];
	char *paddress2;
	char notes[200];
	char *pnotes;
	addr_single_entry *entry;
	entry = new addr_single_entry;


	if (fgets(data, 3999, stdin)==NULL)
	{
		error("process_single(): Processing error.");
		return;
	}
	pname = finddata(100, data, "name=", name);
	htmldecode(name);
	pemail = finddata(100, data, "email=", email);
	htmldecode(email);
	pfax = finddata(100, data, "fax=", fax);
	htmldecode(fax);
	pphone = finddata(100, data, "phone=", phone);
	htmldecode(phone);
	paddress1 = finddata(100, data, "addressline1=", address1);
	htmldecode(address1);
	paddress2 = finddata(100, data, "addressline2=", address2);
	htmldecode(address2);
	pnotes = finddata(200, data, "notes=", notes);
	htmldecode(notes);

	entry->name = pname;
	entry->email_address = email;
	entry->fax = fax;
	entry->phone = phone;
	entry->address_line_1 = address1;
	entry->address_line_2 = address2;
	entry->notes = notes;
	entry->uid = number;

	//incoming name is not a letter, do not allow it as a name
	if (isalpha(name[0])==0)
	{
		address();
		return;
	}

	if ((*addr_update_single_addr)(plugindata, entry))
	{
		error("reprocess_single(): Error with address book.");
		return;
	}
	//don't need to delete the other vars because it is on
	//the stack not the heap.
	delete entry;

	disjavascript("?book", "Entree entered", "address book");

	return;
}
void add_name_inbox()
{
	char *env;
	int number = 0;
	mb_message *message;

	if ((env = getenv("QUERY_STRING"))==NULL)
	{
		error("add_name_inbox(): Environment error.");
		return;
	}

	number = atoi(env+15);

	message = (*mb_get_message)(plugindata, MB_INBOX, number);

	if (message->reply_to != NULL)
	{
		add_single_addr(message->reply_to, -2);
	}
	else if (message->from != NULL)
	{
		add_single_addr(message->from, -2);
	}
	else
	{
		error("add_name_inbox(): Return address not found.");
		return;
	}

	(*mb_free_message)(message);
	return;
}
void add_group_addr(int email)
{
	int addedit;
	addr_group_entry *groupinfo;

	if (email > 0)
	{
		groupinfo = (*addr_get_group)(plugindata, email);
		if (groupinfo == NULL)
		{
			error("add_group_addr(): Can't open book for read.");
			return;
		}
		addedit = 1;
	}
	else
	{
		groupinfo = new addr_group_entry;
		groupinfo->name = NULL;
		groupinfo->email_addresses = NULL;
		addedit = 0;
	}


	cout << "Content-type: text/HTML" << endl;
	cout << "Pragma: No-Cache" << endl << endl;

	groupinfo->uid = email;

	(*display_address_add_group_head)(plugindata, groupinfo, addedit);

	if (email > 0)
	{
		cout << groupinfo->email_addresses;
	}

	(*display_address_add_group_tail)(plugindata, groupinfo, addedit);


	if (email > 0)
	{
		(*addr_free_addr_group_entry)(groupinfo);
	}
	else
	{
		delete groupinfo;
	}

	return;
}
void process_group()
{
	char data[6000];

	char name[400];
	char *pname;
	char email[6000];
	char *pemail;
	addr_group_entry *entry;

	if (fgets(data, 5999, stdin)==NULL)
	{
		error("process_group(): Post Error.");
		return;
	}

	pname = finddata(400, data, "group=", name);
	htmldecode(name);
	pemail = finddata(6000, data, "addresses=", email);
	htmldecode(email);

	if (isalpha(name[1])==0)
	{
		address();
		return;
	}

	entry = new addr_group_entry;
	entry->name = name;
	entry->email_addresses = email;
	entry->uid = ADDR_UID_ADD;
	if ((*addr_update_group_addr)(plugindata, entry))
	{
		//error occured
		error("process_group(): Error opening gaddressbook tmp.");
		return;
	}
	delete entry;

	disjavascript("?book", "Entree added", "address book");

	return;

}
void delete_addr() //remove address
{
	int number = 0; //one to delete
	char *env;

	if ((env = getenv("QUERY_STRING"))==NULL)
	{
		address();
		return;
	}
	number = atoi(env+12);

	(*addr_delete_single_addr)(plugindata, number);
	disjavascript("?book", "Address deleted", "address book");

	return;
}

void delete_group_addr()
{
	int number = 0;
	char *env;

	if ((env = getenv("QUERY_STRING"))==NULL)
	{
		error("delete_group_addr: Error reading environment.");
		return;
	}
        number = atoi(env+18);

	(*addr_delete_group_addr)(plugindata, number);

	disjavascript("?book", "Address deleted", "address book");

	return;
}
void s_sendto_addr() //sets up write to send mail
{
	char *env;
	int number = 0;
	addr_single_entry *address;

	if ((env = getenv("QUERY_STRING")) == NULL)
	{
		error("s_sendto_addr(): Environment error.");
		return;
	}
	number = atoi(env+14);

	address = (*addr_get_single)(plugindata, number);
	wewrite(2, address->email_address, 0, 0);
	(*addr_free_addr_single_entry)(address);

	return;
}

void g_sendto_addr()
{
	char *env;
	int number = 0;
	addr_group_entry *entry;

	if ((env = getenv("QUERY_STRING")) == NULL)
	{
		error("g_sendto_addr(): Environment error.");
		return;
	}
	number = atoi(env+14);
	entry = (*addr_get_group)(plugindata, number);

	int i = 0;
	while (entry->email_addresses[i] != '\0')
	{
		if (entry->email_addresses[i] == '\n')
		{
			entry->email_addresses[i] = ',';
		}
		i++;
	}


	wewrite(2, entry->email_addresses, 0, 0);
	(*addr_free_addr_group_entry)(entry);

	return;
}
void edit_group_addr()
{
	char *env;
	int number = 0;

	if ((env = getenv("QUERY_STRING")) == NULL)
	{
		error("edit_group_addr(): Environment error.");
		return;
	}

	number = atoi(env+16);

	add_group_addr(number);

	return;
}
void reprocess_group()
{
	char *env;
	int number = 0;
	if ((env = getenv("QUERY_STRING")) == NULL)
	{
		error("reprocess_group(): Environment error.");
		return;
	}
	number = atoi(env+16);

	char data[6000];

	char name[400];
	char *pname;
	char email[6000];
	char *pemail;
	addr_group_entry *entry;

	if (fgets(data, 5999, stdin)==NULL)
	{
		error("process_group(): Post Error.");
		return;
	}

	pname = finddata(400, data, "group=", name);
	htmldecode(name);
	pemail = finddata(6000, data, "addresses=", email);
	htmldecode(email);

	if (isalpha(name[1])==0)
	{
		address();
		return;
	}

	entry = new addr_group_entry;
	entry->name = name;
	entry->email_addresses = email;
	entry->uid = number;
	if ((*addr_update_group_addr)(plugindata, entry))
	{
		//error occured
		error("process_group(): Error opening gaddressbook tmp.");
		return;
	}
	delete entry;

	disjavascript("?book", "Entree added", "address book");

	return;
}

