#include "kdtree.h"
#include "sort.h"
#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

int main(){

  double all_points[8][2] = {{0.2,0.01},{0.3,1.0},{0.1,2.0},{0.19101,3.0},{0.6,5.0},{0.8,5.0},{0.9,0.0013},{0.51,3.4}};
  const double *points = &all_points[0][0];
  
  double query_point[2] = {0.0,5.0};
  double query_point2[2] = {4.0,2.0};
  const double *query = query_point;
  const double *query2 = query_point2;

  int k = 5;
  int d = 2;
  int n = 8;

  struct kdtree *kdtree = kdtree_create(d, n, points);
  //knn(k,d,n,points,query2);


  return 1;
}

struct node {
  int point_index;
  int axis;
  struct node *left;
  struct node *right;
};

struct kdtree {
  int d;
  const double *points;
  struct node *root;
};

struct node* kdtree_create_node(int d, const double *points, int depth, int n, int *indexes) {
  struct node *current_node = malloc(sizeof(struct node));
  printf("\nCH 1");
  int axis = depth % d;

  double *remaining_points = malloc(n*d*sizeof(double));

  double *axis_values = malloc(n*sizeof(double));

  for(int row = 0; row < n; ++row){
    printf("\nrow: %d", row);
    for(int col = 0; col < d; ++col){
      remaining_points[(row*d)+col] = *(points + (row * d) + col);
      printf("\nhere: %f", *(points + (row * d) + col));
      if(col == axis){
        axis_values[row] = *(points + (row * d) + col);
      }
    }
  }
  for(int row = 0; row < n; ++row){
    printf("\naxis_values[n]: %f",*(axis_values + row));
    printf("\nIndexes: %d",*(indexes + row));
  }

  for(int i = 0; i < n; ++i){
    for(int j = 0; j < n; ++j){
      if ((axis_values[j]<axis_values[i])){
				double tmp = axis_values[i];   
        int i_tmp = indexes[i];      
				axis_values[i] = axis_values[j];            
				axis_values[j] = tmp; 
        indexes[i] = indexes[j];
        indexes[j] = i_tmp;
			}  
    }
  }
  double middle = round((n-1)/2.0);
  int median_index = (int)middle;
  int median = *(indexes + median_index);


  for(int row = 0; row < n; ++row){
      printf("\nafter sort axis_values[n]: %f",*(axis_values + row));
      printf("\nIndexes after sort: %d",*(indexes + row));
  }
  //printf("\nMedian: %d",median);

  printf("\nMedian: %d",median);

  double *remaining_points_left = malloc((middle - 1)*d*sizeof(double));
  double *remaining_points_right = malloc((n - middle + 1)*d*sizeof(double));
  int n_points_left = (n - middle - 1);
  int n_points_right = (n - (middle + 1));
  printf("\nn_points_left: %d",n_points_left);
  printf("\nn_points_right: %d",n_points_right);

  for(int i=0; i < n; ++i){
    int current_index = *(indexes + i);
    for(int dim = 0; dim < d; ++dim){
      if(i < median_index){
        *(remaining_points_left + (i * d) + dim) = *(points + (current_index * d) + dim);
      }
      else if(i > median_index){
        *(remaining_points_right + (i * d) + dim) = *(points + (current_index * d) + dim);
      }
    }
  }




  return current_node;
}

struct kdtree *kdtree_create(int d, int n, const double *points) {
  struct kdtree *tree = malloc(sizeof(struct kdtree));
  tree->d = d;
  tree->points = points;
  int *indexes = malloc(sizeof(int) * n);

  for (int i = 0; i <= n; i++) {
    indexes[i] = i;
  }

  tree->root = kdtree_create_node(d, points, 0, n, indexes);

  free(indexes);

  return 0;
}

void kdtree_free_node(struct node *node) {
  assert(0);
}

void kdtree_free(struct kdtree *tree) {
  kdtree_free_node(tree->root);
  free(tree);
}

void kdtree_knn_node(const struct kdtree *tree, int k, const double* query,
                     int *closest, double *radius,
                     const struct node *node) {
  assert(0);
}

int* kdtree_knn(const struct kdtree *tree, int k, const double* query) {
  int* closest = malloc(k * sizeof(int));
  double radius = INFINITY;

  for (int i = 0; i < k; i++) {
    closest[i] = -1;
  }

  kdtree_knn_node(tree, k, query, closest, &radius, tree->root);

  return closest;
}

void kdtree_svg_node(double scale, FILE *f, const struct kdtree *tree,
                     double x1, double y1, double x2, double y2,
                     const struct node *node) {
  if (node == NULL) {
    return;
  }

  double coord = tree->points[node->point_index*2+node->axis];
  if (node->axis == 0) {
    // Split the X axis, so vertical line.
    fprintf(f, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke-width=\"1\" stroke=\"black\" />\n",
            coord*scale, y1*scale, coord*scale, y2*scale);
    kdtree_svg_node(scale, f, tree,
                    x1, y1, coord, y2,
                    node->left);
    kdtree_svg_node(scale, f, tree,
                    coord, y1, x2, y2,
                    node->right);
  } else {
    // Split the Y axis, so horizontal line.
    fprintf(f, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke-width=\"1\" stroke=\"black\" />\n",
            x1*scale, coord*scale, x2*scale, coord*scale);
    kdtree_svg_node(scale, f, tree,
                    x1, y1, x2, coord,
                    node->left);
    kdtree_svg_node(scale, f, tree,
                    x1, coord, x2, y2,
                    node->right);
  }
}

void kdtree_svg(double scale, FILE* f, const struct kdtree *tree) {
  assert(tree->d == 2);
  kdtree_svg_node(scale, f, tree, 0, 0, 1, 1, tree->root);
}
