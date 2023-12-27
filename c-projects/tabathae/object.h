#pragma once

#include <stdlib.h>

typedef struct {
  double x, y, z;
} vertice_t;

typedef struct {
  vertice_t **vertices;
  size_t vertices_amount;
} face_t;

typedef struct {
  vertice_t *vertices;
  size_t vertices_amount;
  face_t *faces;
  size_t faces_amount;
} object_t;

typedef enum {
  OK,
  OUT_OF_MEMORY,
  INVALID_ARGUMENTS,
  CALCULATION_ERROR,
} object_errors_t;

/*****************************
           parsing
*****************************/
int object_from_file(char *filepath, object_t **object);

/****************************
   affine transformations
****************************/

int move_x(object_t *object, double distance);
int move_y(object_t *object, double distance);
int move_z(object_t *object, double distance);

int rotate_x(object_t *object, double angle);
int rotate_y(object_t *object, double angle);
int rotate_z(object_t *object, double angle);

int scale_x(object_t *object, double scale_factor);
int scale_y(object_t *object, double scale_factor);
int scale_z(object_t *object, double scale_factor);

/*************************
          utils
*************************/
object_t *create_object(void);
void destroy_object(object_t *object);
