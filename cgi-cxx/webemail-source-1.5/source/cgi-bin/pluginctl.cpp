#include <dlfcn.h>
#include "webemail.h"
void setup_display_plugin();
void setup_mb_plugin();
void setup_addr_plugin();

void plugin_error(char *error)
{
	cout << "Content-type: text/plain" << endl << endl;
	cout << "Error while trying to load plugin." << endl;
	cout << "Please contact your sys adm with the following error." << endl;
	cout << error;
}
void setup_plugin()
{
	setup_display_plugin();
	setup_mb_plugin();
	setup_addr_plugin();
	return;
}
void setup_addr_plugin()
{
	char *error;
	if (addr_plugin=dlopen(plugindata->addr_plugin,RTLD_LAZY))
	{
		if (!addr_plugin)
		{
			fputs (dlerror(), stderr);
			fputs (" Bad address plugin\n", stderr);
			plugin_error("Bad address Plugin!");
			exit(1);
		}
		addr_get_interface_version=(int(*)())dlsym(addr_plugin, "get_interface_version");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("addr Can't load version function.");
			exit(1);
		}

		if ((*addr_get_interface_version)() != 1)
		{
			fputs (" Wrong interface version. Something other than 1.\n", stderr);
			plugin_error("Wrong plugin version!");
			exit(1);
		}
		addr_get_plugin_subsystem=(int (*)())dlsym(addr_plugin, "get_plugin_subsystem");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		if ((*addr_get_plugin_subsystem)() != 3)
		{
			fputs (" Wrong subsystem version. Something other than 3.\n", stderr);
			plugin_error("Wrong subsystem!");
			exit(1);
		}
		addr_get_plugin_version=(double(*)())dlsym(addr_plugin,
		"get_plugin_version");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		addr_get_plugin_name=(char *(*)())dlsym(addr_plugin,
		"get_plugin_name");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		addr_free_mem=(void (*)(void *))dlsym(addr_plugin, "free_mem");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
 		addr_free_addr_single_entry=(void (*)(addr_single_entry *))dlsym(addr_plugin,
		"free_addr_single_entry");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		addr_free_addr_group_entry=(void (*)(addr_group_entry *))dlsym(addr_plugin,
		"free_addr_group_entry");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		addr_free_addr_list=(void (*)(addr_list *))dlsym(addr_plugin,
		"free_addr_list");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		addr_get_addr_list=(addr_list* (*)(plugin_data *))dlsym(addr_plugin,
		"get_addr_list");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		addr_get_group=(addr_group_entry* (*)(plugin_data *, int))dlsym(addr_plugin,
		"get_group");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		addr_get_single=(addr_single_entry* (*)(plugin_data *, int))dlsym(addr_plugin,
		"get_single");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		addr_update_single_addr=(int (*)(plugin_data *, addr_single_entry *))dlsym(addr_plugin,
		"update_single_addr");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		addr_update_group_addr=(int (*)(plugin_data *, addr_group_entry *))dlsym(addr_plugin,
		"update_group_addr");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		addr_delete_single_addr=(void (*)(plugin_data *, int))dlsym(addr_plugin,
		"delete_single_addr");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		addr_delete_group_addr=(void (*)(plugin_data *, int))dlsym(addr_plugin,
		"delete_group_addr");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}

	}
	else
	{
		fputs (dlerror(), stderr);
		fputs (" Bad address book plugin\n", stderr);
		plugin_error("Bad address book Plugin!");
		exit(1);
	}
}
void setup_mb_plugin()
{

	char *error;
	if (mb_plugin=dlopen(plugindata->mb_plugin,RTLD_LAZY))
	{
		if (!mb_plugin)
		{
			fputs (dlerror(), stderr);
			fputs (" Bad mailbox plugin\n", stderr);
			plugin_error("Bad Mailbox Plugin!");
			exit(1);
		}

		mb_get_interface_version=(int(*)())dlsym(mb_plugin, "get_interface_version");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("MB Can't load version function.");
			exit(1);
		}

		if ((*mb_get_interface_version)() != 2)
		{
			fputs (" Wrong interface version. Something other than 2.\n", stderr);
			plugin_error("Wrong plugin version!");
			exit(1);
		}
		mb_get_plugin_subsystem=(int (*)())dlsym(mb_plugin, "get_plugin_subsystem");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		if ((*mb_get_plugin_subsystem)() != 2)
		{
			fputs (" Wrong subsystem version. Something other than 2.\n", stderr);
			plugin_error("Wrong subsystem!");
			exit(1);
		}


		mb_get_plugin_version=(double(*)())dlsym(mb_plugin,
		"get_plugin_version");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		mb_get_plugin_name=(char *(*)())dlsym(mb_plugin,
		"get_plugin_name");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		mb_free_mem=(void (*)(void *))dlsym(mb_plugin,
		"free_mem");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		mb_free_message=(void (*)(mb_message*))dlsym(mb_plugin,
		"free_message");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		mb_get_message=(mb_message* (*)(plugin_data*, int, int))dlsym(mb_plugin,
		"get_message");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		mb_mark=(void (*)(plugin_data*, int, int, int, int))dlsym(mb_plugin,
		"mark");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		mb_get_message_list=(mb_list* (*)(plugin_data*, int))dlsym(mb_plugin,
		"get_message_list");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		mb_free_message_list=(void (*)(mb_list *))dlsym(mb_plugin,
		"free_message_list");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		mb_over_quota=(int (*)(plugin_data *, int, int))dlsym(mb_plugin,
		"over_quota");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		mb_delete_message=(void (*)(plugin_data *, int, int))dlsym(mb_plugin,
		"delete_message");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		mb_multi_delete_message=(void (*)(plugin_data *, int,
		int *, int))dlsym(mb_plugin,
		"multi_delete_message");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		mb_copy_message=(void(*)(plugin_data *, int, int, int))
		dlsym(mb_plugin, "copy_message");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		};
		mb_move_message=(void(*)(plugin_data *, int, int, int))
		dlsym(mb_plugin, "move_message");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		};
		mb_add_message=(void(*)(plugin_data *, int, mb_message *))
		dlsym(mb_plugin, "add_message");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		};
		mb_get_attachment=(int (*)(struct plugin_data *, int, int, int, FILE *))
		dlsym(mb_plugin, "get_attachment");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		};
		mb_delete_all_messages=(void (*)(struct plugin_data *, int))
		dlsym(mb_plugin, "delete_all_messages");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		};
	}
	else
	{
		fputs (dlerror(), stderr);
		fputs (" Bad Mail box plugin\n", stderr);
		plugin_error("Bad mail box Plugin!");
		exit(1);
	}


}


void setup_display_plugin()
{
	char *error;
	if (display_plugin=dlopen(plugindata->display_plugin,RTLD_LAZY))
	{
		if (!display_plugin)
		{
			fputs (dlerror(), stderr);
			fputs (" Bad Display plugin\n", stderr);
			plugin_error("Bad Display Plugin!");
			exit(1);
		}

		get_interface_version=(int (*)())dlsym(display_plugin, "get_interface_version");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Can't load version function.");
			exit(1);
		}

		if ((*get_interface_version)() != 4)
		{
			fputs (" Wrong interface version. Something other than 4.\n", stderr);
			plugin_error("Wrong plugin version!");
			exit(1);
		}
		get_plugin_subsystem=(int (*)())dlsym(display_plugin, "get_plugin_subsystem");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		if ((*get_plugin_subsystem)() != 1)
		{
			fputs (" Wrong subsystem version. Something other than 1.\n", stderr);
			plugin_error("Wrong subsystem!");
			exit(1);
		}


		get_plugin_version=(double(*)())dlsym(display_plugin,
		"get_plugin_version");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		get_language=(char *(*)())dlsym(display_plugin,
		"get_language");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		get_plugin_name=(char *(*)())dlsym(display_plugin,
		"get_plugin_name");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_main_window=(void (*)(plugin_data*, char*))dlsym(display_plugin,
		"display_main_window");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_main_title=(void (*)(plugin_data*))dlsym(display_plugin,
		"display_main_title");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_main_menu=(void (*)(plugin_data*))dlsym(display_plugin,
		"display_main_menu");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}

		display_menu_head=(void (*)(plugin_data*))dlsym(display_plugin,
		"display_menu_head");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_menu_entree=(void (*)(plugin_data*, char*, char*, char*, int))dlsym(display_plugin,
		"display_menu_entree");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_menu_tail=(void (*)(plugin_data*, int, int, int, int))dlsym(display_plugin,
		"display_menu_tail");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_saved_menu_head=(void (*)(plugin_data*))dlsym(display_plugin,
		"display_saved_menu_head");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_saved_menu_entree=(void (*)(plugin_data*, char*, char*, char*, int))dlsym(display_plugin,
		"display_saved_menu_entree");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_saved_menu_tail=(void (*)(plugin_data*, int, int, int, int))dlsym(display_plugin,
		"display_saved_menu_tail");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_sent_menu_head=(void (*)(plugin_data*))dlsym(display_plugin,
		"display_sent_menu_head");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_sent_menu_entree=(void (*)(plugin_data*, char*, char*, char*, int))dlsym(display_plugin,
		"display_sent_menu_entree");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_sent_menu_tail=(void (*)(plugin_data*, int, int, int, int))dlsym(display_plugin,
		"display_sent_menu_tail");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}

		display_read_head=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_read_head");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_read_middle=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_read_middle");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_read_attachments_head=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_read_attachments_head");
	   	if ((error = dlerror()) != NULL)
 		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
 			exit(1);
		}
		display_read_attachments=(void (*)(plugin_data*, int, int, char*, char*))dlsym(display_plugin,
		"display_read_attachments");
	   	if ((error = dlerror()) != NULL)
 		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
 			exit(1);
		}
		display_read_attachments_tail=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_read_attachments_tail");
	   	if ((error = dlerror()) != NULL)
 		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
 			exit(1);
		}
		display_read_tail=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_read_tail");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_saved_read_head=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_saved_read_head");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_saved_read_middle=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_saved_read_middle");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_saved_read_attachments_head=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_saved_read_attachments_head");
	   	if ((error = dlerror()) != NULL)
 		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
 			exit(1);
		}
		display_saved_read_attachments=(void (*)(plugin_data*, int, int, char*, char*))dlsym(display_plugin,
		"display_saved_read_attachments");
	   	if ((error = dlerror()) != NULL)
 		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
 			exit(1);
		}
		display_saved_read_attachments_tail=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_saved_read_attachments_tail");
	   	if ((error = dlerror()) != NULL)
 		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
 			exit(1);
		}
		display_saved_read_tail=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_saved_read_tail");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_sent_read_head=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_sent_read_head");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_sent_read_middle=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_sent_read_middle");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_sent_read_attachments_head=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_sent_read_attachments_head");
	   	if ((error = dlerror()) != NULL)
 		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
 			exit(1);
		}
		display_sent_read_attachments=(void (*)(plugin_data*, int, int, char*, char*))dlsym(display_plugin,
		"display_sent_read_attachments");
	   	if ((error = dlerror()) != NULL)
 		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
 			exit(1);
		}
		display_sent_read_attachments_tail=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_sent_read_attachments_tail");
	   	if ((error = dlerror()) != NULL)
 		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
 			exit(1);
		}
		display_sent_read_tail=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_sent_read_tail");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_write_head=(void (*)(plugin_data*, wewrite_data *))dlsym(display_plugin,
		"display_write_head");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_write_tail=(void (*)(plugin_data*, wewrite_data *))dlsym(display_plugin,
		"display_write_tail");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_config=(void (*)(plugin_data*, config_data*))dlsym(display_plugin,
		"display_config");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_about=(void (*)(plugin_data*))dlsym(display_plugin,
		"display_about");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_login=(void (*)(plugin_data*))dlsym(display_plugin,
		"display_login");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_error=(void (*)(plugin_data*, char *))dlsym(display_plugin,
		"display_error");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_address_head=(void (*)(plugin_data*))dlsym(display_plugin,
		"display_address_head");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_address_letterbreak=(void (*)(plugin_data*, char))dlsym(display_plugin,
		"display_address_letterbreak");
 		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_single_entre=(void (*)(plugin_data*, addr_single_entry*))dlsym(display_plugin,
		"display_single_entre");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_group_entre=(void (*)(plugin_data*, addr_group_entry*))dlsym(display_plugin,
		"display_group_entre");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_address_tail=(void (*)(plugin_data*))dlsym(display_plugin,
		"display_address_tail");
		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_address_add_group_head=(void (*)(plugin_data*, addr_group_entry*, int))dlsym(display_plugin,
		"display_address_add_group_head");
 		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_address_add_group_tail=(void (*)(plugin_data*, addr_group_entry*, int))dlsym(display_plugin,
		"display_address_add_group_tail");
 		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_address_add_single=(void (*)(plugin_data*, addr_single_entry*, int))dlsym(display_plugin,
		"display_address_add_single");
 		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_view_html_head=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_view_html_head");
 		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_saved_view_html_head=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_saved_view_html_head");
 		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_sent_view_html_head=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_sent_view_html_head");
 		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_view_html_tail=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_view_html_tail");
 		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_saved_view_html_tail=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_saved_view_html_tail");
 		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_sent_view_html_tail=(void (*)(plugin_data*, int))dlsym(display_plugin,
		"display_sent_view_html_tail");
 		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}

		display_java_script_head=(void (*)(plugin_data*))dlsym(display_plugin,
		"display_java_script_head");
 		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}
		display_java_script_tail=(void (*)(plugin_data*))dlsym(display_plugin,
		"display_java_script_tail");
 		if ((error = dlerror()) != NULL)
		{
			fputs(error, stderr);
			plugin_error("Function load problem!");
			exit(1);
		}

	}
	else
	{
		fputs (dlerror(), stderr);
		fputs (" Bad display plugin\n", stderr);
		plugin_error("Bad Display Plugin!");
		exit(1);
	}
}

void close_plugin()
{
	dlclose(display_plugin);
	dlclose(mb_plugin);
}

