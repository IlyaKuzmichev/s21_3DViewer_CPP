#pragma once

#include "../object.h"

int compare_numbers(double na, double nb);
int compare_vertices(vertice_t av, vertice_t bv);
int compare_faces(face_t af, face_t bf);
int compare_objects(object_t *ao, object_t *bo);

int affine_transformations_test(void);
int object_builder_test(void);
int parsing_object_test(void);

object_t *construct_test_cube(vertice_t vertices[8]);
object_t *construct_basic_test_cube(void);
