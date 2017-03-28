#include <criterion/criterion.h>
#include "convert.h"

Test(test, temp) {
  cr_assert( compare_eps(0.f, convert_temp_F_to_C(32), 1e-4) );
  cr_assert( compare_eps(32.f, convert_temp_C_to_F(0), 1e-4) );
}

Test(test, rev) {
  cr_assert_str_eq("edcba", str_reverse("abcde"));
}

Test(test, ws) {
  cr_assert_str_eq("", strip_outerws(""));
  cr_assert_str_eq("", strip_outerws(NULL));
  cr_assert_str_eq("", strip_outerws("           "));
  cr_assert_str_eq("cat", strip_outerws("     cat     "));
  cr_assert_str_eq("cat", strip_outerws("cat        "));
  cr_assert_str_eq("cat", strip_outerws("            cat"));
}

Test(test, spl) {
  const char* const a [] = {
    "abc", "def", "ghi"
  };
  char** b = str_split2("abc.def;ghij", ".;j", NULL, NULL);
  cr_assert_arr_eq(a, b, 3);
}
