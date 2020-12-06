#include "util.h"
#include <math.h>
#include <stdio.h>
#include <assert.h>

double distance(int d, const double *x, const double *y) {
  double sum = 0.0;
  for (int i = 0; i < d; ++i){//for every dimension
    sum += pow((x[i] - y[i]),2);
  }
  return sqrt(sum);
}

int insert_if_closer(int k, int d, const double *points, int *closest, const double *query, int candidate) {                    
  double candidate_p[d]; 
  int updated = 0;//returns 0 if no changes were made
  for (int i = 0; i < d; ++i){
    candidate_p[i] = *(points+(candidate * d)+i); //get the actual points of the candidate
  }            
  const double *candidate_points = candidate_p;          
  double candidate_distance = distance(d,candidate_points,query); //calculate the candidates distance to the query point

  for (int i = 0; i < k; ++i){ //for every index in the closest array
    int current_index = closest[i];
    if(current_index == -1){
      *(closest + i) = candidate; //place the candidate if index is available (-1)
      updated = 1;
      break;
    }
    else{ //if the index already has an assigned point
      double current_p[d];
      for (int m = 0; m < d; ++m){
        current_p[m] = *(points+(current_index * d)+m);//get the actual point values 
      }
      const double *current_points = current_p;
      double current_distance = distance(d,current_points,query);//calculate the distance of that point to query
      if(candidate_distance < current_distance){//if the candidate is actually closer
        int holder[k];
        for (int c = 0; c < k; ++c){
          holder[c] = *(closest + c);//save the current layout of the "closest" array
        }     
        for (int c = i; c < k; ++c){
          *(closest + c + 1) = holder[c];//move every point that is further away than candidate one space to the right
        }
        *(closest + i) = candidate;//place the candidate in the index where it should be
        updated = 1;
        break;
      }

    }
  }
  return updated;
}






