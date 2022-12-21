#ifndef READCONFIG_H
#define READCONFIG_H

struct data_entree
{
        struct data_entree *next;
        char *data; //string containing key word
};

struct config_info
{
        struct config_info *next;
        struct data_entree *entree;
};

struct config_info * read_config(char *filename);
void free_config(struct config_info *data);

#endif
