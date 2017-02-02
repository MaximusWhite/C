#ifndef VECTOR_H_
#define VECTOR_H_

typedef struct {
  int *item;
  int size;
} Vector;


void VectorRead(Vector *V);

#endif
