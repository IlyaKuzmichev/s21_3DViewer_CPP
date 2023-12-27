#include "test_utils.h"

int main(void) {
  int status = object_builder_test();
  status += affine_transformations_test();
  status += parsing_object_test();
  return status;
}
