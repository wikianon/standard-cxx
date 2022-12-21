#include "../../addrplugin.h"
#include "../../plugin.h"
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

//--------------------------
//built in functions
//--------------------------

int lock_flock(FILE *file);
int unlock_flock(FILE *file);

int unlock_fcntl(FILE *file);
int lock_fcntl(FILE *file, char type);

void checkgroup(char * data) //preventes emails from haveing -
{
	int i = 0;
	if (data[0] == '-')
	{
		data[0] = '!';
	}
	while (data[i] != '\0')
	{
		if (data[i] == '\n')
		{
			if (data[i+1] == '-')
			{
				data[i+1] = '!';
			}
		}
		i++;
	}

	return;
}
void* append_to(char * dest, char *src)
{
	if (dest != NULL)
	{
		dest = realloc(dest, strlen(dest)+strlen(src)+1);
		strncpy(dest+strlen(dest), src, strlen(src)+1);
	}
	else
	{
		dest = malloc(strlen(src)+1);
		strncpy(dest, src, strlen(src)+1);
	}
	return dest;
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


int bothlock(FILE *file, char type)
{
	if (lock_flock(file))
		return 1;
	if (lock_fcntl(file, type))
		return 2;
	return 0;
}

int bothunlock(FILE *file)
{
	fsync(fileno(file));

	if (unlock_flock(file))
		return 1;
	if (unlock_fcntl(file))
		return 2;
	return 0;
}

int lock_fcntl(FILE *file, char type)
{
	struct flock lock;
	if (type == 'w')
		lock.l_type = F_WRLCK;
	else if (type == 'r')
		lock.l_type = F_RDLCK;
	else
		return 1;

	lock.l_whence = SEEK_SET;
	lock.l_start = lock.l_len=0;

	if (fcntl(fileno(file), F_SETLKW, &lock) < 0)
	{
		return 1;
	}
	return 0;
}

int unlock_fcntl(FILE *file)
{
	struct flock lock;
	lock.l_type = F_UNLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = lock.l_len=0;

	if (fcntl(fileno(file), F_SETLKW, &lock) < 0)
	{
		return 1;
	}
	return 0;
}

int lock_flock(FILE *file)
{
	if (flock(fileno(file), LOCK_EX)<0)
		return 1;
	return 0;

}
int unlock_flock(FILE *file)
{
	if (flock(fileno(file), LOCK_UN)<0)
		return 1;
	return 0;
}

//--------------------------
//interface functions
//--------------------------

//default plugin interface
int get_interface_version()
{
	return 1;
}
int get_plugin_subsystem()
{
	return 3;  //address book sub system.
}
double get_plugin_version()
{
	return 0.9;
}
char* get_plugin_name()
{
	char *name;
	name = malloc(20);
	strncpy(name, "old", 19);
	return name;
}
//frees allocated memory
void free_mem(void *data)
{
	free(data);
	return;
}

void free_addr_single_entry(struct addr_single_entry *entry)
{
	if (entry->name != NULL)
		free(entry->name);
	if (entry->email_address != NULL)
		free(entry->email_address);
	if (entry->address_line_1 != NULL)
		free(entry->address_line_1);
	if (entry->address_line_2 != NULL)
		free(entry->address_line_2);
	if (entry->phone != NULL)
		free(entry->phone);
	if (entry->fax != NULL)
		free(entry->fax);
	if (entry->notes != NULL)
		free(entry->notes);

	free(entry);
}
void free_addr_group_entry(struct addr_group_entry *entry)
{
	if (entry->name != NULL)
		free(entry->name);
	if (entry->email_addresses != NULL)
		free(entry->email_addresses);
	free(entry);
}
void free_addr_list(struct addr_list *list)
{
	struct addr_list *current;
	struct addr_list *next;

	current = list;
	while (current != NULL)
	{
		if (current->type == ADDR_GROUP)
			free_addr_group_entry((struct addr_group_entry*)(current->entry));
		if (current->type == ADDR_SINGLE)
			free_addr_single_entry((struct addr_single_entry*)(current->entry));
		next = current->next;
		free(current);
		current = next;
	}
}

//retrieving of info
struct addr_list* get_addr_list(struct plugin_data *plugindata)
{

	FILE *s_address;
	FILE *g_address;
	char s_filename[400];
	char g_filename[400];

	struct passwd *info;

	char name[100];
	char email[100];
	char phone[100];
	char fax[100];
	char address1[100];
	char address2[100];
	char notes[100];

	char ginfo[400];

	int group_done = 0;
	int group_empty = 1;
	int single_done = 0;
	int single_empty = 1;
	char group_letter;
	char single_letter;

	int s_count = 0;
	int g_count = 0;
	struct addr_group_entry *groupinfo;
	struct addr_single_entry *addressinfo;
	struct addr_list *list = NULL;
	struct addr_list *current = NULL;
	int loop = 0;

	info=getpwnam(plugindata->user_name);
	snprintf(s_filename, 400, "%s/.webmail/book", info->pw_dir);
	snprintf(g_filename, 400, "%s/.webmail/gbook", info->pw_dir);

	s_address = fopen(s_filename, "r");
	if (s_address == NULL)
	{
		//file has problems
		return NULL;
	}
	if (bothlock(s_address, 'r')!=0)
	{
		fclose(s_address);
		//error("address():Error locking book for read");
		return NULL;
	}
	g_address = fopen(g_filename, "r");
	if (g_address == NULL)
	{
		//file not found or has problems.
		bothunlock(s_address);
		fclose(s_address);
		return NULL;
	}
	if (bothlock(g_address, 'r')!=0)
	{
		bothunlock(s_address);
		fclose(s_address);
		fclose(g_address);
		//error("address():Error locking gbook for read");
		return NULL;
	}

	while (!group_done || !single_done)
	{
		loop++;
		if (loop == 20)
			break;

		//read group
		if (group_done == 0 && group_empty)
		{
			groupinfo = malloc(sizeof (struct addr_group_entry));
			do {
				if (fgets(ginfo, 399, g_address) == NULL)
				{
					group_done = 1;
					break;
				}
                        } while (ginfo[0] != '-');
			if (group_done == 0)
			{
				groupinfo->name = malloc(strlen(ginfo)+1);
				strncpy(groupinfo->name, ginfo+1, strlen(ginfo)+1);
				removereturn(groupinfo->name);
				groupinfo->email_addresses = NULL;

				g_count++;
				groupinfo->uid=g_count;
				group_letter = toupper(groupinfo->name[0]);
				group_empty = 0;
			}
		}
		//read single
		if (single_done == 0 && single_empty)
		{
			addressinfo = malloc(sizeof (struct addr_single_entry));
			addressinfo->name = NULL;
			addressinfo->email_address = NULL;
			addressinfo->phone = NULL;
			addressinfo->fax = NULL;
			addressinfo->address_line_1 = NULL;
			addressinfo->address_line_2 = NULL;
			addressinfo->notes = NULL;

			if (fgets(name, 99, s_address) == NULL) single_done = 1;
			addressinfo->name = malloc(strlen(name)+1);
			strncpy(addressinfo->name, name, strlen(name)+1);
			removereturn(addressinfo->name);
			if (fgets(email, 99, s_address) == NULL) single_done = 1;
			addressinfo->email_address = malloc(strlen(email)+1);
			strncpy(addressinfo->email_address, email, strlen(email)+1);
			removereturn(addressinfo->email_address);
			if (fgets(phone, 99, s_address) == NULL) single_done = 1;
			addressinfo->phone = malloc(strlen(phone)+1);
			strncpy(addressinfo->phone, phone, strlen(phone)+1);
			removereturn(addressinfo->phone);
			if (fgets(fax, 99, s_address) == NULL) single_done = 1;
			addressinfo->fax = malloc(strlen(fax)+1);
			strncpy(addressinfo->fax, fax, strlen(fax)+1);
			removereturn(addressinfo->fax);
			if (fgets(address1, 99, s_address) == NULL) single_done = 1;
			addressinfo->address_line_1 = malloc(strlen(address1)+1);
			strncpy(addressinfo->address_line_1, address1, strlen(address1)+1);
			removereturn(addressinfo->address_line_1);
			if (fgets(address2, 99, s_address) == NULL) single_done = 1;
			addressinfo->address_line_2 = malloc(strlen(address2)+1);
			strncpy(addressinfo->address_line_2, address2, strlen(address2)+1);
			removereturn(addressinfo->address_line_2);
			if (fgets(notes, 99, s_address) == NULL) single_done = 1;
			addressinfo->notes = malloc(strlen(notes)+1);
			strncpy(addressinfo->notes, notes, strlen(notes)+1);
			removereturn(addressinfo->notes);

			s_count++;
			addressinfo->uid=s_count;
			single_letter = toupper(addressinfo->name[0]);
			single_empty = 0;

			if (single_done) //hit end of file while reading, free mem.
			{
				if (addressinfo->name != NULL) free(addressinfo->name);
				if (addressinfo->email_address != NULL) free(addressinfo->email_address);
				if (addressinfo->phone != NULL) free(addressinfo->phone);
				if (addressinfo->fax != NULL) free(addressinfo->fax);
				if (addressinfo->address_line_1 != NULL) free(addressinfo->address_line_1);
				if (addressinfo->address_line_2 != NULL) free(addressinfo->address_line_2);
				if (addressinfo->notes != NULL) free(addressinfo->notes);
				free(addressinfo);
			}
		}
		if (group_done && single_done)
		{
			break;
		}
		//setup data struct

		if (list == NULL) //create head
		{
			list = malloc(sizeof (struct addr_list));
			current = list;
		}
		else //append to head
		{
			current->next = malloc(sizeof (struct addr_list));
			current = current->next;
		}
		current->next = NULL;

		if (single_done && !group_done)
		{
			//append group entry
			current->type = ADDR_GROUP;
			current->entry = groupinfo;
			group_empty = 1;
		}
		else if ( !single_done && group_done)
		{
			current->type = ADDR_SINGLE;
			current->entry = addressinfo;
			single_empty = 1;
		}
		else if (group_letter < single_letter)
		{
			current->type = ADDR_GROUP;
			current->entry = groupinfo;
			group_empty = 1;
		}
		else
		{
			//append single entry
			current->type = ADDR_SINGLE;
			current->entry = addressinfo;
			single_empty = 1;
		}
	}


	bothunlock(g_address);
	fclose(g_address);
	bothunlock(s_address);
	fclose(s_address);

	return list;
}
struct addr_group_entry* get_group(struct plugin_data *plugindata, int uid)
{
	FILE *gbook;
	char gbookname[400];
	struct passwd *info;
	char name[400];
	char data[4000];
	int count = 0;
	struct addr_group_entry *entry;

	entry = malloc(sizeof (struct addr_group_entry));
	entry->name = NULL;
	entry->email_addresses = NULL;

	info=getpwnam(plugindata->user_name);
	snprintf(gbookname, 400, "%s/.webmail/gbook", info->pw_dir);
	gbook = fopen(gbookname, "r");
	if (gbook == NULL)
	{
		fprintf(stderr, "webemail:addr:old:get_group can't open address book.\n");
		free_addr_group_entry(entry);
		return NULL;
	}
	if (bothlock(gbook, 'r')!=0)
	{
		fclose(gbook);
		fprintf(stderr, "webemail:addr:old:update_single_addr can't lock address book.\n");
		free_addr_group_entry(entry);
		return NULL;
	}
	while (1) //finds the name of the group
	{
		if(fgets(name, 399, gbook)==NULL)
		{
			fprintf(stderr, "webemail:addr:old:update_single_addr can't read address book.\n");
			free_addr_group_entry(entry);
			return NULL;
		}
		if (name[0] == '-')
			count++;
		if (count == uid)
			break;
	}
	entry->name = malloc(strlen(name)+1);
	strncpy(entry->name, name+1, strlen(name)+1);

	while (fgets(data, 3999, gbook)!=NULL)
	{
		if (data[0] == '-')
			break;
		entry->email_addresses = append_to(entry->email_addresses, data);
	}
	bothunlock(gbook);
	fclose(gbook);
	return entry;
}
struct addr_single_entry* get_single(struct plugin_data *plugindata, int uid)
{
	FILE *book;
	char bookname[400];
	char buf[500];
	struct passwd *info;
	struct addr_single_entry *entry;
	int i;

	entry = malloc(sizeof (struct addr_single_entry));


	info=getpwnam(plugindata->user_name);
	snprintf(bookname, 400, "%s/.webmail/book", info->pw_dir);
	book = fopen(bookname, "r");
	if (book == NULL)
	{
		fprintf(stderr, "webemail:addr:old:get_single can't open address book.\n");
		return NULL;
	}
	if (bothlock(book, 'r')!=0)
	{
		fclose(book);
		fprintf(stderr, "webemail:addr:old:update_single_addr can't lock address book.\n");
		return NULL;
	}
	for (i = 0; i<(uid-1)*7; i++)
	{
		if(fgets(buf, 499, book)==NULL)
		{
			fprintf(stderr, "webemail:addr:old:update_single_addr can't read address book.\n");
			return NULL;
		}
	}
	//name
	fgets(buf, 499, book);
	entry->name = malloc(strlen(buf)+1);
	removereturn(buf);
	strncpy(entry->name, buf, strlen(buf)+1);
	//email
	fgets(buf, 499, book);
	entry->email_address = malloc(strlen(buf)+1);
	removereturn(buf);
	strncpy(entry->email_address, buf, strlen(buf)+1);
	//fax
	fgets(buf, 499, book);
	entry->fax = malloc(strlen(buf)+1);
	removereturn(buf);
	strncpy(entry->fax, buf, strlen(buf)+1);
	//phone
	fgets(buf, 499, book);
	entry->phone = malloc(strlen(buf)+1);
	removereturn(buf);
	strncpy(entry->phone, buf, strlen(buf)+1);
	//address line 1
	fgets(buf, 499, book);
	entry->address_line_1 = malloc(strlen(buf)+1);
	removereturn(buf);
	strncpy(entry->address_line_1, buf, strlen(buf)+1);
	//address line 2
	fgets(buf, 499, book);
	entry->address_line_2 = malloc(strlen(buf)+1);
	removereturn(buf);
	strncpy(entry->address_line_2, buf, strlen(buf)+1);
	//notes
	fgets(buf, 499, book);
	entry->notes = malloc(strlen(buf)+1);
	removereturn(buf);
	strncpy(entry->notes, buf, strlen(buf)+1);

	entry->uid = uid;

	bothunlock(book);
	fclose(book);

	return entry;
}

//adding / update of info
//If the uid is non zero the current entry is updated
//If the uid is zero the current entry is added.
int update_single_addr(struct plugin_data *plugindata, struct addr_single_entry *entry)
{
	struct passwd *info;
	FILE *book;
	FILE *tmp;
	char filenametmp[200];
	char filename[200];
	char data[200];
	int inserted = 0;
	int i;
	int count = 0; //the position of the message in the file
			//this is the entries uid.

	info=getpwnam(plugindata->user_name);
	snprintf(filename, 200, "%s/.webmail/book", info->pw_dir);
	snprintf(filenametmp, 200, "%s/.webmail/tmp", info->pw_dir);
	book = fopen(filename, "r");
	if (book == NULL)
	{
		fprintf(stderr, "webemail:addr:old:update_single_addr can't open address book.\n");
		return 1;
	}
	if (bothlock(book, 'r')!=0)
	{
		fclose(book);
		fprintf(stderr, "webemail:addr:old:update_single_addr can't lock address book.\n");
		return 1;
	}
	tmp = fopen(filenametmp, "w");
	if (tmp == NULL)
	{
		fprintf(stderr, "webemail:addr:old:update_single_addr can't open tmp.\n");
		return 1;
	}
	if (bothlock(tmp, 'w')!=0)
	{
		fclose(book);
		fclose(tmp);
		fprintf(stderr, "webemail:addr:old:update_single_addr can't lock tmp.\n");
		return 1;
	}

	while (1)
	{
		if (fgets(data, 199, book)==NULL)
			break;
		count++; //count message for its uid
		if (toupper(data[0]) > toupper(entry->name[0]) && inserted == 0)
		{
			fputs(entry->name, tmp);
			fputs("\n", tmp);
			fputs(entry->email_address, tmp);
			fputs("\n", tmp);
			fputs(entry->phone, tmp);
			fputs("\n", tmp);
			fputs(entry->fax, tmp);
			fputs("\n", tmp);
			fputs(entry->address_line_1, tmp);
			fputs("\n", tmp);
			fputs(entry->address_line_2, tmp);
			fputs("\n", tmp);
			fputs(entry->notes, tmp);
			fputs("\n", tmp);
			inserted = 1;
		}

		if (entry->uid == count)  //this message is being updated, don't
		{	                  //add it back to the data base.
			for (i = 0; i < 6; i++)
			{
				if (fgets(data, 199, book)==NULL)
					break;
			}
			continue;
		}
		//We are at a new entry, copy the entry to the new file
		fputs(data, tmp);//puts name
		for (i = 0; i < 6; i++)
		{
			if (fgets(data, 199, book)==NULL)
				break;
			fputs(data, tmp); //puts the rest of the data
		}
	}
	if (inserted == 0)
	{
		fputs(entry->name, tmp);
		fputs("\n", tmp);
		fputs(entry->email_address, tmp);
		fputs("\n", tmp);
		fputs(entry->phone, tmp);
		fputs("\n", tmp);
		fputs(entry->fax, tmp);
		fputs("\n", tmp);
		fputs(entry->address_line_1, tmp);
		fputs("\n", tmp);
		fputs(entry->address_line_2, tmp);
		fputs("\n", tmp);
		fputs(entry->notes, tmp);
		fputs("\n", tmp);
	}


	fflush(tmp);
	fdatasync(fileno(tmp));
	bothunlock(tmp);
	fclose(tmp);

	bothunlock(book);
	fclose(book);

	rename(filenametmp, filename);
	sync();
	return 0; //everything went fine
}
int update_group_addr(struct plugin_data *plugindata, struct addr_group_entry *entry)
{
	struct passwd *info;
	FILE *book;
	FILE *tmp;
	char filenametmp[200];
	char filename[200];
	int inserted = 0;
	int count = 0;
	char data[400];
	info=getpwnam(plugindata->user_name);
	snprintf(filename, 200, "%s/.webmail/gbook", info->pw_dir);
	snprintf(filenametmp, 200, "%s/.webmail/tmp", info->pw_dir);
	book = fopen(filename, "r");

	if (book == NULL)
	{
		fprintf(stderr, "webemail:addr:old:update_group_addr Can't open address book.\n");
		return 1;
	}
	if (bothlock(book, 'r')!=0)
	{
		fclose(book);
		fprintf(stderr, "webemail:addr:old:update_group_addr Can't open address book.\n");
		return 1;
	}
	tmp = fopen(filenametmp, "w");
	if (tmp == NULL)
	{
		bothunlock(book);
		fclose(book);
		fprintf(stderr, "webemail:addr:old:update_single_addr Can't open address book.\n");
		return 1;
	}
	if (bothlock(tmp, 'w')!=0)
	{
		bothunlock(book);
		fclose(book);
		fclose(tmp);
		fprintf(stderr, "webemail:addr:old:update_group_addr Can't open address book.\n");
		return 1;
	}

	while (1)
	{
		if (fgets(data, 399, book)==NULL)
			break;
		if (data[0] == '-')
		{
			count++;
			if (toupper(data[1]) > toupper(entry->name[0]) && inserted == 0)
			{
				fputs("-", tmp);
				fputs(entry->name, tmp);
				fputs("\n", tmp);
				fputs(entry->email_addresses, tmp);
				fputs("\n", tmp);
				inserted = 1;
			}
		}
		if (count != entry->uid)  //erase old data.
			fputs(data, tmp);//puts data
	}
	if (inserted == 0)
	{
		fputs("-", tmp);
		fputs(entry->name, tmp);
		fputs("\n", tmp);
		checkgroup(entry->email_addresses);
		fputs(entry->email_addresses, tmp);
		fputs("\n", tmp);
	}


	fflush(tmp);
	fdatasync(fileno(tmp));
	bothunlock(tmp);
	fclose(tmp);
	bothunlock(book);
	fclose(book);

	rename(filenametmp, filename);
	sync();

	return 0;
}

//removing of the address info
void delete_single_addr(struct plugin_data *plugindata, int uid)
{
	struct passwd *info;
	FILE *book;
	FILE *tmp;
	char bookname[400];
	char tmpname[400];
	int count = 0;
	char data[500];

	info=getpwnam(plugindata->user_name);
	snprintf(bookname, 400, "%s/.webmail/book", info->pw_dir);
	snprintf(tmpname, 400, "%s/.webmail/tmp", info->pw_dir);

	book = fopen(bookname, "r");
	if (book == NULL)
	{
		fprintf(stderr, "webemail:addr:old:delete_single_addr Can't open address book.\n");
		return;
	}
	if (bothlock(book, 'r')!=0)
	{
		fclose(book);
		fprintf(stderr, "webemail:addr:old:delete_single_addr Can't lock address book.\n");
		return;
	}
	tmp = fopen(tmpname, "w");
	if (tmp == NULL)
	{
		bothunlock(book);
		fclose(book);
		fprintf(stderr, "webemail:addr:old:delete_single_addr Can't open tmp.\n");
		return;
	}
	if (bothlock(tmp, 'w')!=0)
	{
		bothunlock(book);
		fclose(book);
		fclose(tmp);
		fprintf(stderr, "webemail:addr:old:delete_single_addr Can't lock tmp.\n");
		return;
	}

	while(fgets(data, 499, book)!=NULL)
	{
		count++;
		if (count < (uid-1)*7+1 || count > uid*7)
			fputs(data, tmp);
	}

	bothunlock(book);
	fclose(book);
	fflush(tmp);
	fdatasync(fileno(tmp));
	bothunlock(tmp);
	fclose(tmp);

	rename(tmpname, bookname);
	sync();
	return;
}
void delete_group_addr(struct plugin_data *plugindata, int uid)
{
	struct passwd *info;
	FILE *gbook;
	FILE *tmp;
	char gbookname[1000];
	char tmpname[1000];
	char buf[1000];
	int emailcount = 0;

	info=getpwnam(plugindata->user_name);
	snprintf(gbookname, 1000, "%s/.webmail/gbook", info->pw_dir);
	snprintf(tmpname, 1000, "%s/.webmail/tmp", info->pw_dir);
	rename(gbookname, tmpname);

	sync();

	gbook = fopen(gbookname, "w");
	if (gbook == NULL)
	{
		fprintf(stderr, "webemail:addr:old:delete_group_addr Can't open gbook.\n");
		return;
	}
	if (bothlock(gbook, 'w')!=0)
	{
		fclose(gbook);
		fprintf(stderr, "webemail:addr:old:delete_group_addr Can't lock gbook.\n");
		return;
	}
	tmp = fopen(tmpname, "r");
	if (tmp == NULL)
	{
		bothunlock(gbook);
		fclose(gbook);
		fprintf(stderr, "webemail:addr:old:delete_group_addr Can't open tmp.\n");
		return;
	}
	if (bothlock(tmp, 'r')!=0)
	{
		bothunlock(gbook);
		fclose(gbook);
		bothunlock(tmp);
		fclose(tmp);
		fprintf(stderr, "webemail:addr:old:delete_group_addr Can't lock tmp.\n");
		return;
	}

	while (fgets(buf, 999, tmp) != NULL)
	{
		if (buf[0] == '-')
		{
			emailcount++;
		}
		if (emailcount != uid)
		{
			fputs(buf, gbook);
		}
	}
	bothunlock(tmp);
	fclose(tmp);
	fflush(gbook);
	fdatasync(fileno(gbook));
	bothunlock(gbook);
	fclose(gbook);

	return;
}


