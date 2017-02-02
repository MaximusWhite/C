#ifndef PATH_
#define PATH_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
       int *item; 
       int size;  
       int end_index; // index representing the position of the last entry 
} Path;

void PathInit(Path *P, int size); 
int PathAddEntry(Path *P, int entry );
int PathRemoveEntry(Path *P );
void PathPrint(Path P);



#endif

