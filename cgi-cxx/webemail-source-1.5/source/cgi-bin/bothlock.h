#ifndef _bothlock_h
#define _bothlock_h

int bothlock(FILE *file, char type);
int bothunlock(FILE *file);
FILE* processlock();
void processunlock(FILE *file);

#endif
