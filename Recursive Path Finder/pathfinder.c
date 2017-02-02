#include <stdlib.h>
#include <stdio.h>
#include "vector.h"
#include "path.h"


int AllPathsRec(int position, Vector *V, Path *Solution){   //  Recursive function finding possible ways to get from 0 position of Vector to last position of Vector
           						    //  and saves them in Path

  Path *Copy = malloc (sizeof(*Copy));   // creating a copy of Path already obtained
  
  PathInit(Copy, Solution->size); // initializing it

  CopyPath(Solution, Copy);   // copying entries and index

  if (position == V->size-1) {     // the most important base case -> if the end of vector found, print obtained path and return true

    if (!PathAddEntry(Copy, position)){exit(0); }    //  add the last entry to the path (if can't -> exit with error)
   
    PathPrint(*Copy);   // print Path obtained
   
    free(Copy);  //  whenever Path is successfuly found, free the memory holding the cope of the Path obtained before
   
    return 1;
  }  

  if ((position < 0) || (position > V->size-1)) { return 0;} // base case: if the position in Vector is out of boundaries return false

  if ( isInPath(position, Copy)==1 && position!=0 ) {return 0;} // base case : if the current position in Vector was already in Path, that means program goes in a loop, so return false
   
  if ( !PathAddEntry(Copy, position) ) {  //  if none of base cases fit, the position should be added to Path 

    exit(0);   // if can't -> exit with error 
  }  
   
  if (AllPathsRec(position + V->item[position], V, Copy)) {  //  call the recursion for next item on the RIGHT and check if it returns true

    AllPathsRec(position - V->item[position], V, Copy);     //  if it does,  still call recursion for the LEFT item 

    return 1;                                               // and return true to the main caller
  }
  if (AllPathsRec(position - V->item[position], V, Copy)) { 

    AllPathsRec(position + V->item[position], V, Copy); 
    return 1;
  }

}
  
int main(int argc, char* args[]){       // main function

int i;

Vector *V = malloc(sizeof(Vector*));   // allocating memory for a Vector

Path *P = malloc(sizeof(Path*));      // allocating memory for a Path

VectorRead(V);   // reading input from stdin

PathInit(P, V->size);  // initializing the Path

if (!AllPathsRec(0, V, P)) printf("NO SOLUTION\n");  // calling recursive function, if returns false -> there are no solutions

free(V);  // freeing memory allocated for Vector and Path 
free(P);

return 1; // done 
}
