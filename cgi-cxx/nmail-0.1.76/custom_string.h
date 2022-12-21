/*
	custom_string.h
*/

#ifndef _CUSTOM_STRING_H
#define _CUSTOM_STRING_H


/*
	SOME STANDARD FUNCTIONS

   char* strchr(const char* string, int c);
	//searches forward
	//returns pointer to first occurance of character c
	//   returns NULL if c not found

   char* strrchr(const char* string, int c);
	//searches backward 
	//returns pointer to last occurance of c
	//   returns NULL if c not found

   char* strstr(const char* haystack, const char* needle);
	//searches forward 
	//returns pointer to substring needle found within haystack
	//   returns NULL if not found

   size_t strspn(const char* string, const char* skipset);
	//searches forward
	//returns length of start of string that consists entirely of
	//   characters specified by skipset

   size_t strcspn(const char* string, const char* stopset);
	//searches forward
	//returns length of start of string that consists of characters
	//   that are not members of stopset

   char* strpbrk(const char* string, const char* stopset);
	//searches forward
	//returns pointer to first char in stopset that occurs in string
	//   returns NULL if no stop char found

*/

/*	CUSTOM FUNCTIONS	*/

char* strpcbrk(const char* string, const char* skipset);
	//searches forward
	//returns pointer to first char in string that is not member
	//   of skipset
	//   returns NULL if all characters are members of skipset

char* strrpbrk(const char* string, const char* stopset);
	//searches string backwards 
	//returns pointer to last char in string that is member of stopset
	//   returns NULL if no characters in stopset found

char* strrpcbrk(const char* string, const char* skipset);
	//searches string backwards
	//returns pointer to last char that is not member of skipset
	//   returns NULL if all characters are members of skipset

#endif
