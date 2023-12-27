#pragma once

#ifndef PARSER
#define PARSER
#define VERTICES_PATTERN "v %lf %lf %lf"
#define FACES_PATTERN "f %d %d %d"
#define NUMBERS "0123456789"
#endif

#define _GNU_SOURCE

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "object.h"
#include "object_builder.h"

int parse_to_builder(FILE* file, object_builder_t* object_builder);
int parse_vertice(char* line, object_builder_t* object_builder);
int parse_face(char* line, object_builder_t* object_builder);
int reallocate_memory(int** array, int* capacity, int cur_index);
void skip_to_space(char* line, int* k);
int vertice_index(char* line, int* k);