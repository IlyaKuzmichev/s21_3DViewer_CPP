#include "object_parser.h"

int object_from_file(char* filepath, object_t** object) {
  int error = OK;
  if (NULL == filepath || NULL == object) {
    error = INVALID_ARGUMENTS;
  } else {
    FILE* file = fopen(filepath, "r");
    if (NULL == file) {
      error = INVALID_ARGUMENTS;
    } else {
      object_builder_t* object_builder = create_object_builder();
      error = parse_to_builder(file, object_builder);
      fclose(file);
      if (error == OK) {
        error = get_object(object_builder, object);
      }
      destroy_object_builder(object_builder);
    }
  }
  return error;
}

int parse_to_builder(FILE* file, object_builder_t* object_builder) {
  int error = OK;
  if (NULL == object_builder) {
    error = INVALID_ARGUMENTS;
  } else {
    char* line = NULL;
    size_t len = 0;
    ssize_t read = 0;
    while ((read = getline(&line, &len, file)) != -1 && error == OK) {
      if (strncmp(line, "v ", 2) == 0) {
        error = parse_vertice(line, object_builder);
      } else if (strncmp(line, "f ", 2) == 0) {
        error = parse_face(line, object_builder);
      }
    }
    free(line);
  }
  return error;
}

int parse_vertice(char* line, object_builder_t* object_builder) {
  int error = OK;
  double x, y, z;
  int count_vertices = sscanf(line, VERTICES_PATTERN, &x, &y, &z);
  if (count_vertices == 3) {
    error = add_vertice_to_object(object_builder, x, y, z);
  } else {
    error = INVALID_ARGUMENTS;
  }
  return error;
}

int parse_face(char* line, object_builder_t* object_builder) {
  int error = OK;
  int* vertices_indexes = calloc(1, sizeof(int));
  int k = 2, cur_index = 0, capacity = 1, space_flag = 1;
  while (line[k] != '\n' && line[k] != '\r' && line[k] != EOF && line[k] != 0 &&
         line[k] != '#' && error == OK) {
    if (line[k] == ' ') {
      k++;
      space_flag = 1;
    } else if (line[k] == '/' && !space_flag) {
      skip_to_space(line, &k);
      space_flag = 0;
    } else if ((strchr(NUMBERS, line[k]) != NULL || line[k] == '-' ||
                line[k] == '+') &&
               space_flag) {
      int index = vertice_index(line, &k);
      vertices_indexes[cur_index++] = index;
      reallocate_memory(&vertices_indexes, &capacity, cur_index);
      space_flag = 0;
    } else {
      error = INVALID_ARGUMENTS;
    }
  }

  if (error == OK)
    error = add_face_to_object(object_builder, vertices_indexes, cur_index);
  if (vertices_indexes) free(vertices_indexes);
  return error;
}

int reallocate_memory(int** array, int* capacity, int cur_index) {
  int error = OK, *tmp = *array;
  if (cur_index == *capacity) {
    *capacity *= 2;
    *array = realloc(*array, *capacity * sizeof(int));
  }
  if (NULL == array) {
    free(tmp);
    error = OUT_OF_MEMORY;
  }
  return error;
}

void skip_to_space(char* line, int* k) {
  while (line[*k] != 0 && !isspace(line[*k]) && line[*k] != '#') {
    (*k)++;
  }
}

int vertice_index(char* line, int* k) {
  long result = 0;
  int sign = 1;
  switch (line[*k]) {
    case '-':
      sign = -1;
      // fallthrough
    case '+':
      ++(*k);
      break;
  }
  int overflow = 0;
  while (strchr(NUMBERS, line[*k]) != NULL && line[*k] != 0 && !overflow) {
    result = result * 10 + (line[*k] - '0');
    overflow = result >> 32;
    (*k)++;
  }
  if (overflow) {
    result = 0;
  }
  result *= sign;
  return (int)result;
}
