#include <stdlib.h>
#include <stdio.h>
#include "path.h"
#include <string.h>

void CopyPath(Path *source, Path *tgt){   // copies a Path from source into target

   int i;
   for (i = 0; i< source->size; i++){  // going through source path entries and copying them into target path
   
      tgt->item[i] = source->item[i];
   }  
   tgt->end_index= source-> end_index;  //  copy the index pointing to the last entry in path


}


void PathInit(Path *P, int size){    // initializes a Path, given it's size

 int i;    
 P->size = size;   // initialize a size of path
 P->item = malloc(sizeof(int) * size);  // allocating memory for entries array
 P->end_index=0;   // setting last entry point index to 0
   
   for (i =0; i< size; i++) {

   P->item[i] = 0;  // filling entries with 0's

 }

}

int PathAddEntry(Path *P, int entry ){   // adds entry to the Path if possible, if not ->  displays message and returns 0


  if (P->end_index< P->size){  // if index is legal 
  P->item[P->end_index] = entry;   // add entry to the current position
  P->end_index++;   // increment index
    
  return 1;  // return successful
  }else {printf("INDEX OVERFLOW :::  END_INDEX = %d  SIZE = %d\n", P->end_index, P->size); return 0;}
}

int PathRemoveEntry(Path *P ){    // Never used that :/

   if (P->end_index>0) P->end_index--;   // if index > 0, just move index 1 left
   if (P->end_index==0) P->item[0]=0;    //  if index = 0, set element @ 0 to 0 
   return 1; 

}

void PathPrint(Path P){  // printing the Path
 
 int i;

 for (i = 0; i<P.end_index; i++){   // going from first position to the last entry in Path
 
  printf("%d ", P.item[i]);      // display it

 }
 printf("\n");     // add next line to make it smooth 
 
}

int isInPath(int tgt, Path *P){    // indicates if the target value is in Path

  int i;
for (i = 0; i< P->end_index; i++){   // going through entries
       
     if (tgt == P->item[i]) return 1;  // if one of them is equal to target, return true
}
   
 return 0;  // otherwise not found
}
     


