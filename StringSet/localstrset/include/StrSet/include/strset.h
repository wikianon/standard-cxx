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

#include "../src/rdspc/rdspc.c"
#include "../src/iostr/iostr.c"
#include "../src/strjst/strjst.c"
#include "../src/rmspc/rmspc.c"
#include "../src/space/space.c"
#include "../src/add/add.c"
#include "../src/histogram/histograma.c"
#include "../src/rvrs/rvrs.c"
#endif 
