#include <criterion/criterion.h>
#include "convert.h"

Test(test, temp) {
  cr_assert( compare_eps(0.f, convert_temp_F_to_C(32), 1e-4) );
  cr_assert( compare_eps(32.f, convert_temp_C_to_F(0), 1e-4) );
}

Test(test, ws) {
  cr_assert_str_eq("", strip_outerws("     "));
  cr_assert_str_eq("", strip_outerws(""));
  cr_assert_str_eq("cat", strip_outerws("     cat       "));
}
