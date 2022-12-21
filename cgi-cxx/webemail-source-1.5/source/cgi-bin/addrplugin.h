#ifndef ADDRPLUGIN_H
#define ADDRPLUGIN_H

//If the uid is non zero the current entry is updated
//If the uid is zero the current entry is added.
#define ADDR_SINGLE 0
#define ADDR_GROUP 1

#define ADDR_UID_ADD 0

struct addr_single_entry
{
	char* name;
	char* email_address;
	char* address_line_1;
	char* address_line_2;
	char* phone;
	char* fax;
	char* notes;
	int uid;
};

struct addr_group_entry
{
	char *name;
	char *email_addresses;  //each email is seperated by a \n
				//the whole thing is null terminated
	int uid;
};

struct addr_list
{
	struct addr_list *next;
	//only one should be used, is selected with type

	void *entry;
	int type; // 0 = single, 1 = group
	//ADDR_SINGLE 0
	//ADDR_GROUP 1

};

#endif
