#include <iostream>


char *chrncsnt(const char* str)
{
 char* char_not_const=(char*)str;
 //char* char_not_const=const_cast<char*>(str);
 return char_not_const;
}

const char *csntchr(char* str)
{
 const char* const_char=(const char*)str;
 //const char* const_char=const_cast<const char*>(str);
 return const_char;
}

std::string chrcsntstdstr(const char* dat)
{
 return std::string(dat);
}

const char * strcsntchr(std::string my)
{
 const char *dat = my.c_str();
 return dat;
}

int main()
{ 
 char *frase1;
 
 const char* minhafrase="Hello World";
 
 frase1 = chrncsnt(minhafrase);
 
 printf("\n\t%s\n\n",frase1);
 
 const char* frase2 = csntchr(frase1);
 
 printf("\n\t%s\n\n",frase2);
 
 const char* dat = "my string!";
 std::string instdstring = chrcsntstdstr(dat);
 std::cout << "\n\t" << instdstring << "\n\n";
 
 std::string my_string2="testing!";
 
 dat = strcsntchr(my_string2);
 std::cout << "\n\t" << dat << "\n\n";
 
 return 0;
}