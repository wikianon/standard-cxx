#ifndef _STRSET_H
#define _STRSET_H

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>









void add(const char*);
int  space(char*);
char rmspc(char*); //OK
char *rdspc(char*); //OK
char *strjst(char*); //OK
void iostr(char*); //OK
void histogram(char*); //ok 
char* rvrs(char*); //ok










#include <StrSet/src/rdspc/rdspc.c>
#include <StrSet/src/iostr/iostr.c>
#include <StrSet/src/strjst/strjst.c>
#include <StrSet/src/rmspc/rmspc.c>
#include <StrSet/src/space/space.c>
#include <StrSet/src/add/add.c>
#include <StrSet/src/histogram/histograma.c>
#include <StrSet/src/rvrs/rvrs.c>

#endif