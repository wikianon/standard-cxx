#ifndef _CBUFF_CXX
#define _CBUFF_CXX

#include "cbuff.hxx"







/*
#if defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)

void cbuff(void)
{
 fflush(stdin);
}

#elif defined(__linux__) || defined(__gnu_linux__)

void cbuff(void)
{
 __fpurge(stdin);
}

#else

void cbuff(void)
{
 int ch;
 while((ch=getchar()) != EOF && ch != '\n');
}

#endif
*/
void cbuff(void)
{
#if defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)

 fflush(stdin);

#elif defined(__linux__) || defined(__gnu_linux__)

__fpurge(stdin);

#else

 int ch;
 while((ch=getchar()) != EOF && ch != '\n');

#endif
}





#endif //_CBUFF_CXX