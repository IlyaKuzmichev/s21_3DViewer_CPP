#include "test_utils.h"

#include <check.h>
#include <math.h>

#include "../object.h"
#include "../object_builder.h"

static const double cmp_epsilon = 1e-7;

int compare_numbers(double na, double nb) {
  return fabs(na - nb) < cmp_epsilon;
}

int compare_vertices(vertice_t av, vertice_t bv) {
  return compare_numbers(av.x, bv.x) && compare_numbers(av.y, bv.y) &&
         compare_numbers(av.z, bv.z);
}

int compare_faces(face_t af, face_t bf) {
  int result = af.vertices_amount == bf.vertices_amount;
  for (size_t i = 0; i < af.vertices_amount && result; ++i) {
    vertice_t *av = af.vertices[i], *bv = bf.vertices[i];
    if (NULL == av || NULL == bv || !compare_vertices(*av, *bv)) {
      result = 0;
    }
  }
  return result;
}

int compare_objects(object_t *ao, object_t *bo) {
  if (NULL == ao || NULL == bo) {
    return !ao && !bo;
  }
  int result = 1;
  if (ao->vertices_amount != bo->vertices_amount) {
    result = 0;
  } else if (ao->faces_amount != bo->faces_amount) {
    result = 0;
  }

  for (size_t i = 0; i < ao->vertices_amount && result; ++i) {
    if (!compare_vertices(ao->vertices[i], bo->vertices[i])) {
      result = 0;
    }
  }

  for (size_t i = 0; i < ao->faces_amount && result; ++i) {
    if (!compare_faces(ao->faces[i], bo->faces[i])) {
      result = 0;
    }
  }

  return result;
}

object_t *construct_test_cube(vertice_t vertices[8]) {
  object_builder_t *builder = create_object_builder();
  ck_assert_ptr_nonnull(builder);

  for (size_t i = 0; i < 8; ++i) {
    int error = add_vertice_to_object(builder, vertices[i].x, vertices[i].y,
                                      vertices[i].z);
    ck_assert_int_eq(OK, error);
  }

  int vertices_indexes[4] = {1, 2, 3, 4};
  int error = add_face_to_object(builder, vertices_indexes, 4);
  ck_assert_int_eq(OK, error);

  for (int i = 0; i < 4; ++i) {
    int vertices_indexes[4] = {i + 1, (i + 1) % 4 + 1, i + 5, (i + 1) % 4 + 5};
    error = add_face_to_object(builder, vertices_indexes, 4);
    ck_assert_int_eq(OK, error);
  }

  vertices_indexes[0] = 5;
  vertices_indexes[1] = 6;
  vertices_indexes[2] = 7;
  vertices_indexes[3] = 8;

  error = add_face_to_object(builder, vertices_indexes, 4);
  ck_assert_int_eq(OK, error);

  object_t *object = NULL;
  error = get_object(builder, &object);
  ck_assert_int_eq(OK, error);
  ck_assert_ptr_nonnull(object);

  destroy_object_builder(builder);

  return object;
}

object_t *construct_basic_test_cube(void) {
  vertice_t vertices[8] = {
      {1., 1., 0.}, {-1., 1., 0.}, {-1., -1., 0.}, {1., -1., 0.},
      {1., 1., 2.}, {-1., 1., 2.}, {-1., -1., 2.}, {1., -1., 2.},
  };
  return construct_test_cube(vertices);
}
