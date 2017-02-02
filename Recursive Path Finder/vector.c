#include <stdlib.h>
#include <stdio.h>
#include "vector.h"
#include <string.h>


char *shrinkString(char* str){     // throwing away the first number from a line

  char *tmp = malloc(sizeof(char) * 256);  // temp var for line
  
  int i=0;    // this variable indicates the index of the first space occurence
   
  while(str[i]!=' ') {    // while space is not encountered yet increase the index
   if(str[i]== '\n') return "";    //if the end of line encountered return empty string
   i++;   
  }
  strncpy(tmp, str+i+1, strlen(str)-i-1);  // copy the part of the source string starting from the first symbol AFTER the first space into temp
  return tmp; // return shrinked  line
}


void VectorRead(Vector *V){   // Reading input from std

  int i;  // loop index

  char *tmp = malloc(sizeof(char)* 256);    // variable for  first line of input (size of vector)
  char *str = malloc(sizeof(char) * 256);   // variable for second line of input (items) 


  fgets(tmp, 256, stdin);  // getting first line

  V->size = atoi(tmp);  // defining the size of vector

  V->item = malloc(sizeof(int) * V->size);   //  allocating memory for *size* integers

  fgets(str,256,stdin);   // reading the string with numbers

  for (i = 0; i< V->size; i++){  // one by one write obtained numbers into vector
    
   sscanf(str,"%d", &(V->item[i])); // using sscanf  to get one number at a time  from the beginning of the line
   str = shrinkString(str);  // shrink the line, i.e. throw away the first read number from it

  }
  

}
