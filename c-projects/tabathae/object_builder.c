#include "object_builder.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "object.h"

static const double EXPANDING_FACTOR = 1.4;
static const size_t INITIAL_CAPACITY = 8;

object_builder_t *create_object_builder(void) {
  object_builder_t *builder = malloc(sizeof(object_builder_t));
  if (NULL == builder) {
    return NULL;
  }
  builder->vertices_capacity = 0;
  builder->faces_capacity = 0;
  builder->object = NULL;
  return builder;
}

int init_object(object_builder_t *builder) {
  int error = OK;
  builder->object = create_object();
  if (NULL == builder->object) {
    error = OUT_OF_MEMORY;
  }
  return error;
}

int init_object_vertices(object_builder_t *builder) {
  int error = OK;
  builder->vertices_capacity = INITIAL_CAPACITY;
  builder->object->vertices =
      calloc(builder->vertices_capacity, sizeof(vertice_t));
  if (NULL == builder->object->vertices) {
    error = OUT_OF_MEMORY;
  }
  return error;
}

int init_object_faces(object_builder_t *builder) {
  int error = OK;
  builder->faces_capacity = INITIAL_CAPACITY;
  builder->object->faces = calloc(builder->faces_capacity, sizeof(face_t));
  if (NULL == builder->object->faces) {
    error = OUT_OF_MEMORY;
  }
  return error;
}

void clear_builder(object_builder_t *builder) {
  if (NULL != builder) {
    destroy_object(builder->object);
    builder->object = NULL;
    builder->vertices_capacity = 0;
    builder->faces_capacity = 0;
  }
}

int object_lazy_initialization(object_builder_t *builder) {
  int error = OK;
  if (NULL == builder->object) {
    if (OK != init_object(builder) || OK != init_object_vertices(builder) ||
        OK != init_object_faces(builder)) {
      clear_builder(builder);
      error = OUT_OF_MEMORY;
    }
  }
  return error;
}

void *safe_realloc(void *ptr, size_t newsize) {
  void *old_ptr = ptr;
  ptr = realloc(ptr, newsize);
  if (NULL == ptr) {
    free(old_ptr);
  }
  return ptr;
}

void *expand(void *ptr, size_t *capacity, size_t elemsize) {
  *capacity = (size_t)(*capacity * EXPANDING_FACTOR);
  return safe_realloc(ptr, *capacity * elemsize);
}

bool is_coordinate_valid(double coord) {
  return !(isnan(coord) || isinf(coord));
}

bool are_coordinates_valid(double x, double y, double z) {
  return is_coordinate_valid(x) && is_coordinate_valid(y) &&
         is_coordinate_valid(z);
}

int expand_vertices(object_builder_t *builder) {
  int error = OK;
  if (builder->object->vertices_amount == builder->vertices_capacity) {
    builder->object->vertices =
        expand(builder->object->vertices, &builder->vertices_capacity,
               sizeof(vertice_t));
    if (NULL == builder->object->vertices) {
      error = OUT_OF_MEMORY;
    }
  }
  return error;
}

int expand_faces(object_builder_t *builder) {
  int error = OK;
  if (builder->object->faces_amount == builder->faces_capacity) {
    builder->object->faces = expand(builder->object->faces,
                                    &builder->faces_capacity, sizeof(face_t));
    if (NULL == builder->object->faces) {
      error = OUT_OF_MEMORY;
    }
  }
  return error;
}

int prepare_adding_vertice(object_builder_t *builder) {
  int error = object_lazy_initialization(builder);
  if (OK == error) {
    error = expand_vertices(builder);
  }
  return error;
}

int add_vertice_to_object(object_builder_t *builder, double x, double y,
                          double z) {
  int error = OK;
  if (!are_coordinates_valid(x, y, z) || NULL == builder) {
    error = INVALID_ARGUMENTS;
  } else {
    error = prepare_adding_vertice(builder);
  }
  if (OK == error) {
    builder->object->vertices[builder->object->vertices_amount] =
        (vertice_t){.x = x, .y = y, .z = z};
    ++builder->object->vertices_amount;
  } else {
    clear_builder(builder);
  }
  return error;
}

bool are_add_face_arguments_valid(object_builder_t *builder,
                                  int *vertices_indexes,
                                  size_t vertices_amount) {
  bool result = true;
  if (NULL == builder || NULL == builder->object || vertices_amount == 0) {
    result = false;
  } else {
    for (size_t i = 0; i < vertices_amount && result; ++i) {
      if ((size_t)(abs(vertices_indexes[i]) - 1) >=
              builder->object->vertices_amount ||
          0 == vertices_indexes[i]) {
        result = false;
      }
    }
  }
  return result;
}

int prepare_face(object_builder_t *builder, face_t *face,
                 size_t vertices_amount) {
  int error = expand_faces(builder);
  if (OK == error) {
    *face = (face_t){0};
    face->vertices = calloc(sizeof(vertice_t *), vertices_amount);
    if (NULL == face->vertices) {
      error = OUT_OF_MEMORY;
    }
  }
  return error;
}

int add_face_to_object(object_builder_t *builder, int *vertices_indexes,
                       size_t vertices_amount) {
  int error = OK;
  face_t face = {0};
  if (!are_add_face_arguments_valid(builder, vertices_indexes,
                                    vertices_amount)) {
    error = INVALID_ARGUMENTS;
  } else {
    error = prepare_face(builder, &face, vertices_amount);
  }

  if (OK == error) {
    face.vertices_amount = vertices_amount;
    for (size_t i = 0; i < vertices_amount; ++i) {
      vertice_t *vertice_ptr = NULL;
      if (vertices_indexes[i] < 0) {
        vertice_ptr =
            &builder->object->vertices[builder->object->vertices_amount +
                                       vertices_indexes[i]];
      } else {
        vertice_ptr = &builder->object->vertices[vertices_indexes[i] - 1];
      }
      face.vertices[i] = vertice_ptr;
    }
    builder->object->faces[builder->object->faces_amount] = face;
    ++builder->object->faces_amount;
  } else {
    clear_builder(builder);
  }
  return error;
}

int get_object(object_builder_t *builder, object_t **result) {
  if (NULL == result) {
    return INVALID_ARGUMENTS;
  }
  *result = builder->object;
  builder->object = NULL;
  clear_builder(builder);
  return OK;
}

void destroy_object_builder(object_builder_t *builder) {
  if (NULL != builder) {
    clear_builder(builder);
  }
  free(builder);
}
