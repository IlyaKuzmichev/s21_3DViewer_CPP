#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "3d_viewer.h"

/**
 * parse_error
 * @brief Enumeration of errors, occured in parsing
 */
enum parse_error {
  /// all are right!
  status_ok,
  /// file cannot be opened
  cannot_open_file,
  /// format of file is incorrect
  incorrect_format,
  /// error in allocation of memory
  memory_allocation_error
};

/**
 * @brief Fucntion for parsing .obj file
 *
 * @param[in] path - null-terminated string that contains path to .obj file
 * @param[out] obj - result of parsing process
 *
 * @return error code of parsing (check enum parse_error)
 */
int parse_obj_file(const char *path, object_t *obj);