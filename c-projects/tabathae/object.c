#include "object.h"

#include <math.h>
#include <stdlib.h>

object_t *create_object(void) {
  object_t *object = malloc(sizeof(object_t));
  if (NULL != object) {
    object->vertices = NULL;
    object->vertices_amount = 0;
    object->faces = NULL;
    object->faces_amount = 0;
  }
  return object;
}

void destroy_object(object_t *object) {
  if (NULL != object) {
    free(object->vertices);
    for (size_t i = 0; i < object->faces_amount; ++i) {
      free(object->faces[i].vertices);
    }
    free(object->faces);
    free(object);
  }
}

int is_valid_object(object_t *object) {
  return NULL != object &&
         (0 == object->vertices_amount || NULL != object->vertices) &&
         (0 == object->faces_amount || NULL != object->faces);
}

int is_valid_number(double number) { return !(isinf(number) || isnan(number)); }

int are_args_valid(object_t *object, double number) {
  return is_valid_number(number) && is_valid_object(object);
}

#ifndef COORD_X
#define COORD_X x
#endif

#ifndef COORD_Y
#define COORD_Y y
#endif

#ifndef COORD_Z
#define COORD_Z z
#endif

#ifndef move_object
#define move_object(object, distance, coordinate)                          \
  ({                                                                       \
    int error = (OK);                                                      \
    if (!are_args_valid(object, distance)) {                               \
      error = INVALID_ARGUMENTS;                                           \
    } else {                                                               \
      size_t i = 0;                                                        \
      for (i = 0; i < object->vertices_amount && OK == error; ++i) {       \
        double new_coordinate = object->vertices[i].coordinate + distance; \
        if (!is_valid_number(new_coordinate)) {                            \
          --i;                                                             \
          error = CALCULATION_ERROR;                                       \
        } else {                                                           \
          object->vertices[i].coordinate = new_coordinate;                 \
        }                                                                  \
      }                                                                    \
      if (OK != error) {                                                   \
        for (size_t j = 0; j < i; ++j) {                                   \
          object->vertices[i].coordinate -= distance;                      \
        }                                                                  \
      }                                                                    \
    };                                                                     \
    error;                                                                 \
  })
#endif

int move_x(object_t *object, double distance) {
  return move_object(object, distance, COORD_X);
}

int move_y(object_t *object, double distance) {
  return move_object(object, distance, COORD_Y);
}

int move_z(object_t *object, double distance) {
  return move_object(object, distance, COORD_Z);
}

#ifndef rotate_object
#define rotate_object(object, angle, coordinate1, coordinate2)                 \
  ({                                                                           \
    int error = (OK);                                                          \
    if (!are_args_valid(object, angle)) {                                      \
      error = INVALID_ARGUMENTS;                                               \
    } else {                                                                   \
      for (size_t i = 0; i < object->vertices_amount; ++i) {                   \
        double v_coord_1 = object->vertices[i].coordinate1;                    \
        double v_coord_2 = object->vertices[i].coordinate2;                    \
        double new_coord_1 = cos(angle) * v_coord_1 + sin(angle) * v_coord_2;  \
        double new_coord_2 = -sin(angle) * v_coord_1 + cos(angle) * v_coord_2; \
        object->vertices[i].coordinate1 = new_coord_1;                         \
        object->vertices[i].coordinate2 = new_coord_2;                         \
      }                                                                        \
    };                                                                         \
    error;                                                                     \
  })
#endif

int rotate_x(object_t *object, double angle) {
  return rotate_object(object, angle, COORD_Y, COORD_Z);
}

int rotate_y(object_t *object, double angle) {
  return rotate_object(object, angle, COORD_Z, COORD_X);
}

int rotate_z(object_t *object, double angle) {
  return rotate_object(object, angle, COORD_X, COORD_Y);
}

int is_valid_scale_factor(double scale_factor) {
  return scale_factor > 0 && scale_factor > 1e-7;
}

#ifndef scale_object
#define scale_object(object, scale_factor, coordinate)                         \
  ({                                                                           \
    int error = (OK);                                                          \
    if (!are_args_valid(object, scale_factor) ||                               \
        !is_valid_scale_factor(scale_factor)) {                                \
      error = INVALID_ARGUMENTS;                                               \
    } else {                                                                   \
      size_t i = 0;                                                            \
      for (i = 0; i < object->vertices_amount && OK == error; ++i) {           \
        double new_coordinate = object->vertices[i].coordinate * scale_factor; \
        if (!is_valid_number(new_coordinate)) {                                \
          --i;                                                                 \
          error = CALCULATION_ERROR;                                           \
        } else {                                                               \
          object->vertices[i].coordinate = new_coordinate;                     \
        }                                                                      \
      }                                                                        \
      if (OK != error) {                                                       \
        for (size_t j = 0; j < i; ++j) {                                       \
          object->vertices[i].coordinate /= scale_factor;                      \
        }                                                                      \
      }                                                                        \
    };                                                                         \
    error;                                                                     \
  })
#endif

int scale_x(object_t *object, double scale_factor) {
  return scale_object(object, scale_factor, COORD_X);
}

int scale_y(object_t *object, double scale_factor) {
  int error = scale_object(object, scale_factor, COORD_Y);
  return error;
}

int scale_z(object_t *object, double scale_factor) {
  return scale_object(object, scale_factor, COORD_Z);
}
