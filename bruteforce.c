#include "bruteforce.h"
#include "util.h"
#include <stdlib.h>
#include <assert.h>

int* knn(int k, int d, int n, const double *points, const double* query) {
  int array[k];
  int *closest = &array[0]; //initialize a pointer to an array
  closest = malloc(k*sizeof(int)); //allocate memory
  for(int i = 0; i < k; i++){
    *(closest + i) = -1; //fill the array with -1 to start with
  }
  for(int i = 0; i < n; i++){
    insert_if_closer(k,d,points,closest,query,i); //every reference point goes through here
  }

  return closest;
}

