#include <stdlib.h>
#include <check.h>
#include "../../../src/lib/hashset.h"
#include "../../../src/lib/hashtable.h"

START_TEST(test_hashstr)
{
  // str should hash to:
  //   ((int)'v' + (int)'a' + (int)'r') % HT_DEFAULT_CAPACITY
  char* str = "var";
  char* set_items[] = {"var"};
  HS* hashset = new_hashset(set_items, 1);
  HT* hashtable = new_hashtable();
  int hs_index = hashset_hash(hashset, str);
  int ht_index = hashstr(hashtable, str);
  int expected_index = ((int)'v' + (int)'a' + (int)'r') % HT_DEFAULT_CAPACITY;

  ck_assert_int_eq(hs_index, expected_index);
  ck_assert_int_eq(ht_index, expected_index);

  free_hashset(hashset);
  free_hashtable(hashtable);
}
END_TEST

START_TEST(test_hashset)
{
  // note: x, du, and dx should have the same hash
  char* set_items[] = {"x", "y", "du", "ax"};
  HS* hashset = new_hashset(set_items, 4);

  // check items that are in the set
  ck_assert_int_eq(hashset_find(hashset, "x"), 1);
  ck_assert_int_eq(hashset_find(hashset, "y"), 1);
  ck_assert_int_eq(hashset_find(hashset, "du"), 1);
  ck_assert_int_eq(hashset_find(hashset, "ax"), 1);
  // check item that is not in the set which has no collision
  ck_assert_int_eq(hashset_find(hashset, "not in set"), 0);
  // check item that is not in the set which has a collision
  ck_assert_int_eq(hashset_find(hashset, "bw"), 0);

  free_hashset(hashset);
}
END_TEST

START_TEST(test_exit_hashset_failed_init)
{
  char* set_items[] = {"x", "x"};
  new_hashset(set_items, 2);
}

START_TEST(test_hashtable)
{
  HT* hashtable = new_hashtable();

  // insert example vars --- no hash collision
  hashtable_upsert(hashtable, "x", 0.12);
  hashtable_upsert(hashtable, "y", -345.6);
  // insert example vars --- hash collision with "x"
  // hashstr("x") = 23
  // hashstr("du") = 23
  // hashstr("ax") = 23
  hashtable_upsert(hashtable, "du", -7);
  hashtable_upsert(hashtable, "ax", 8.9);
  
  // check existing vars
  ck_assert_ldouble_eq(hashtable_find(hashtable, "x").value,  0.12);
  ck_assert_ldouble_eq(hashtable_find(hashtable, "y").value,  -345.6);
  ck_assert_ldouble_eq(hashtable_find(hashtable, "du").value, -7);
  ck_assert_ldouble_eq(hashtable_find(hashtable, "ax").value, 8.9);
  // check nonexistant vars
  //   z: no hash collision
  //   bw: hash collision with x
  ck_assert_int_eq(hashtable_find(hashtable, "z").error, 1);
  ck_assert_int_eq(hashtable_find(hashtable, "bw").error, 1);

  // overwrite existing hashtable value
  hashtable_upsert(hashtable, "x", 3.14159265);
  ck_assert_ldouble_eq(hashtable_find(hashtable, "x").value, 3.14159265);

  // expect 4 different variables in the hashtable
  ck_assert_int_eq(hashtable->size, 4);

  free_hashtable(hashtable);
}
END_TEST

Suite * lib_suite(void)
{
  Suite* s;
  TCase* tc_core;

  s = suite_create("Hashtable");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_hashstr);
  tcase_add_test(tc_core, test_hashset);
  tcase_add_exit_test(tc_core, test_exit_hashset_failed_init, 1);
  tcase_add_test(tc_core, test_hashtable);

  suite_add_tcase(s, tc_core);

  return s;
}

int main(void)
{
  int failures;
  Suite* s;
  SRunner* sr;

  s = lib_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  failures = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
