#include <stdio.h>
#include "readconfig.h"

int find_non_space(char *line, int start_index)
{
	int current = start_index;

	while (line[current] == '\t' ||
	line[current] == ' ')
	{
		current++;
	}

	if (line[current] == '\n' ||
	line[current] == '\0' ||
	line[current] == '#')
	{
		return -1;  //no more good info.
	}

	return current;  //here is your starting index
}
int find_key_end(char *line, int start_index)
{
	int current = start_index;
	int in_quotes = 0;

	if (line[current] == '"')
	{
		in_quotes = 1;
		current++;  //first char ok, must move on or we will have problems
	}

	if ((line[current] == '\t' && !in_quotes)||
	(line[current] == ' ' && !in_quotes) ||
	(line[current] == '#' && !in_quotes) ||
	line[current] == '\n' ||
	line[current] == '\0')
	{
		return -1;  //opps something is wrong.
	}


	while ((line[current] != '\t' || in_quotes) &&
	(line[current] != ' ' || in_quotes) &&
	(line[current] != '#' || in_quotes) &&
	line[current] != '\n' &&
	line[current] != '\0')
	{
		if (line[current] == '"') in_quotes = 0;  //finds end quote
		current++;
	}
	return current - 1; //step back one so that we are on the string
}
char * copy_to(char *line, int start, int end)
{
	//take care of quotes
	if (line[start] == '"')
		start++;
	if (line[end] == '"')
		end--;

	int size = (end - start) + 1 + 1;
	if (size <= 0) return NULL;  //can't allocate negative mem. 

	char *tmp = NULL;
	tmp = new char[size];

	//copy data
	int i, j;
	for (i = start, j = 0; i <= end; i++, j++)
	{
		tmp[j] = line[i];
		if (line[i] == '\0') break; //someone messed up.
	}
	tmp[size-1] = '\0'; //make sure it is terminated!
	return tmp;
}



struct config_info * read_config(char *filename)
{
	FILE *config; //file to open
	char line[400]; //line buffer

	//manages the config info struct
	struct config_info *current_config = NULL;
	struct config_info *head_config = NULL;
	struct config_info *tmp_config = NULL;

	//manages the key words in the line
	int index = 0;
	int key_start = 0;
	int key_end = 0;

	//holds the key word
	char *tmp_key = NULL;

	//manages the current entree
	struct data_entree *current_entree = NULL;
	struct data_entree *head_entree = NULL;
	struct data_entree *tmp_entree = NULL;


	config = fopen(filename, "r");
	if (config == NULL)
	{
		return NULL;
	}


	while (fgets(line, 400, config) != NULL)
	{
		//clear key word info
		index = 0;
		key_start = 0;
		key_end = 0;
		tmp_key = NULL;

		//clear entree info
		current_entree = NULL;
		head_entree = NULL;
		tmp_entree = NULL;

		//while we have not found \n \0 or #
		while (index != -1)
		{
			index = find_non_space(line, index);
			if (index == -1) continue;
			key_start = index;

			index = find_key_end(line, index);
			if (index == -1) continue;
			key_end = index;
			index++; //push index to next char

			tmp_key = copy_to(line, key_start, key_end);

			//create data node
			tmp_entree = new data_entree;
			tmp_entree->data = tmp_key;
			tmp_entree->next = NULL;

			if (current_entree == NULL) //add first node
			{
				current_entree = tmp_entree;
				head_entree = current_entree;
			}
			else //place at end
			{
				current_entree->next = tmp_entree;
				current_entree = tmp_entree;
			}
		}
		//nothing interesting on line skip to next
		if (head_entree == NULL) continue;


		//create config node
		tmp_config = new config_info;
		tmp_config->next = NULL;
		tmp_config->entree = head_entree;

		if (current_config == NULL) //add first node
		{
			current_config = tmp_config;
			head_config = tmp_config;
		}
		else //place at end
		{
			current_config->next = tmp_config;
			current_config = tmp_config;
		}
	}


	fclose(config);
	return head_config;
}

void free_entree(struct data_entree *entree)
{
	if (entree == NULL) //base case
		return;

	free_entree(entree->next); //delete next node

	if (entree->data != NULL)
		delete entree->data; 

	delete entree;
	return;
}

void free_config(struct config_info *data)
{
	if (data == NULL) //base case
		return;

	free_config(data->next);
	free_entree(data->entree);
	delete data;
	return;
}

