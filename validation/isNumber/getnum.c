#include <stdio.h>

int getnum()
{
 int c;
 
 int n = 0;
 while ((c=getchar()) >= '0' && c <= '9')
 n = n*10 + c - '0';
 
 if(c == EOF)
 return -1;
 return n;
} 

int getline(char s[], int lim) /* get line into s, return length */
{
 int c;
 int i = 0;
 
 while(--lim > 0 && (c=getchar()) != EOF && c != '\n')s[i++] = c;
 
 if(c == '\n')
  s[i++] = c;
  s[i] = '\0';
 
 return i;
}