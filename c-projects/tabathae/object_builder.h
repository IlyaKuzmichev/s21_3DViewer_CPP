#pragma once

#include "object.h"

typedef struct {
  object_t *object;
  size_t vertices_capacity, faces_capacity;
} object_builder_t;

/****************************
     builder functions
****************************/
object_builder_t *create_object_builder(void);

int add_vertice_to_object(object_builder_t *builder, double x, double y,
                          double z);
int add_face_to_object(object_builder_t *builder, int *vertices_indexes,
                       size_t vertices_amount);

int get_object(object_builder_t *builder, object_t **result);
void destroy_object_builder(object_builder_t *builder);
