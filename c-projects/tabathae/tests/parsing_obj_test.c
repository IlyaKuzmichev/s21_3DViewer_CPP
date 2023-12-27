#include <check.h>

#include "../object_parser.h"
#include "test_utils.h"

START_TEST(vertices_only) {
  int error = OK;
  char filepath[25] = "tests/for_test1.txt";
  object_t* object = NULL;

  error = object_from_file(filepath, &object);

  ck_assert_ptr_nonnull(object);
  ck_assert_uint_eq(OK, error);
  ck_assert_uint_eq(1, object->vertices_amount);
  ck_assert_uint_eq(0, object->faces_amount);

  destroy_object(object);
}
END_TEST

START_TEST(empty_file) {
  int error = OK;
  char filepath[25] = "tests/for_test2.txt";
  object_t* object = NULL;

  error = object_from_file(filepath, &object);

  ck_assert_ptr_null(object);
  ck_assert_uint_eq(OK, error);

  destroy_object(object);
}
END_TEST

START_TEST(multiple_vertices) {
  int error = OK;
  char filepath[25] = "tests/for_test3.txt";
  object_t* object = NULL;

  error = object_from_file(filepath, &object);

  ck_assert_ptr_nonnull(object);
  ck_assert_uint_eq(OK, error);
  ck_assert_uint_eq(8, object->vertices_amount);
  ck_assert_uint_eq(0, object->faces_amount);

  destroy_object(object);
}
END_TEST

START_TEST(cube_test) {
  int error = OK;
  char filepath[25] = "tests/for_test4.txt";
  object_t* object = NULL;

  error = object_from_file(filepath, &object);

  object_t* expected = construct_basic_test_cube();

  ck_assert_ptr_nonnull(object);
  ck_assert_uint_eq(OK, error);
  ck_assert_int_eq(1, compare_objects(expected, object));

  destroy_object(object);
  destroy_object(expected);
}
END_TEST

START_TEST(azino_test) {
  int error = OK;
  char filepath[25] = "tests/for_test777.txt";
  object_t* object = NULL;

  error = object_from_file(filepath, &object);

  ck_assert_ptr_nonnull(object);
  ck_assert_uint_eq(OK, error);
  ck_assert_uint_eq(8, object->vertices_amount);
  ck_assert_uint_eq(5, object->faces_amount);

  destroy_object(object);
}
END_TEST

START_TEST(error_1_test) {
  int error = OK;
  char filepath[25] = "tests/for_test5.txt";
  object_t* object = create_object();

  error = object_from_file(filepath, &object);

  ck_assert_uint_ne(OK, error);

  destroy_object(object);
}
END_TEST

START_TEST(error_2_test) {
  int error = OK;
  char filepath[25] = "tests/for_test6.txt";
  object_t* object = create_object();

  error = object_from_file(filepath, &object);

  ck_assert_uint_ne(OK, error);

  destroy_object(object);
}
END_TEST

START_TEST(error_3_test) {
  int error = OK;
  char filepath[25] = "tests/no_file.txt";
  object_t* object = create_object();

  error = object_from_file(filepath, &object);

  ck_assert_uint_ne(OK, error);

  destroy_object(object);
}
END_TEST

int parsing_object_test(void) {
  Suite* s1 = suite_create("creation_test");
  TCase* tc1_1 = tcase_create("vertices_only_case");
  TCase* tc1_2 = tcase_create("with_faces_case");
  TCase* tc1_3 = tcase_create("errors_case");
  SRunner* sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, vertices_only);
  tcase_add_test(tc1_1, empty_file);
  tcase_add_test(tc1_1, multiple_vertices);

  suite_add_tcase(s1, tc1_2);
  tcase_add_test(tc1_2, cube_test);
  tcase_add_test(tc1_2, azino_test);

  suite_add_tcase(s1, tc1_3);
  tcase_add_test(tc1_3, error_1_test);
  tcase_add_test(tc1_3, error_2_test);
  tcase_add_test(tc1_3, error_3_test);

  srunner_set_fork_status(sr, CK_NOFORK);

  srunner_run_all(sr, CK_VERBOSE);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}