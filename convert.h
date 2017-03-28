#include <strings.h>
#include "common.h"

typedef long double ldbl_t;

typedef enum {
  TEMP_C,
  TEMP_K,
  TEMP_F,
  TEMP_R,
  NUM_TEMPS
} temp_t;

typedef enum {
  RATE_MPH,
  RATE_FTS,
  RATE_MS,
  RATE_KMH,
  RATE_KTS,
  NUM_RATES
} rate_t;

static ldbl_t return_1_ldbl_arg (const ldbl_t a);
ldbl_t             convert_temp (const ldbl_t x, const temp_t from, const temp_t to);
ldbl_t             convert_rate (const long double x, const rate_t from, const rate_t to) pure const_func;
bool                compare_eps (const ldbl_t a, const ldbl_t b, const ldbl_t eps);
rate_t      convert_str_to_rate (const char* const s);
static int        strncasecmp_c (const char* const s1, const char* const s2, const size_t n);
char*             strip_outerws (const char* const str);

#define define_convert_temp_func(from, to, expression) \
  /* static */ ldbl_t convert_temp_ ## from ## _to_ ## to (const ldbl_t value); \
  pure const_func /* warn_unused static */ ldbl_t convert_temp_ ## from ## _to_ ## to \
  (const ldbl_t value) { return (expression); } int IWILLNOTEXIST ## from ## to

define_convert_temp_func(F, C, (5.f/9.f) * (value - 32) );
define_convert_temp_func(C, F, ((9.f/5.f) * value) + 32 );

define_convert_temp_func(K, C, value - 273.15);
define_convert_temp_func(C, K, value + 273.15);

define_convert_temp_func(R, F, value - 459.69);
define_convert_temp_func(F, R, value + 459.69);

define_convert_temp_func(K, F, convert_temp_C_to_F( convert_temp_K_to_C(value) ) );
define_convert_temp_func(F, K, convert_temp_C_to_K( convert_temp_F_to_C(value) ) );

define_convert_temp_func(C, R, convert_temp_F_to_R( convert_temp_C_to_F(value) ) );
define_convert_temp_func(R, C, convert_temp_F_to_C( convert_temp_R_to_F(value) ) );

define_convert_temp_func(K, R, convert_temp_C_to_R( convert_temp_K_to_C(value) ) );
define_convert_temp_func(R, K, convert_temp_F_to_K( convert_temp_R_to_F(value) ) );

static const long double rate_coeffs [NUM_RATES] [NUM_RATES] = {

  [RATE_MPH] = {
    [RATE_MPH] = 1,
    [RATE_FTS] = 1.46667, // mph to fts
    [RATE_MS]  = 0.44704,
    [RATE_KMH] = 1.609344,
    [RATE_KTS] = 0.8689762,
  },

  [RATE_FTS] = {
    [RATE_MPH] = 0.681818, // fts to mph
    [RATE_FTS] = 1,
    [RATE_MS]  = 0.3048,
    [RATE_KMH] = 1.09728,
    [RATE_KTS] = 0.5924838,
  },

  [RATE_MS] = {
    [RATE_MPH] = 2.23694,
    [RATE_FTS] = 3.28084,
    [RATE_MS]  = 1,
    [RATE_KMH] = 3.6,
    [RATE_KTS] = 1.9438445,
  },

  [RATE_KMH] = {
    [RATE_MPH] = 0.621371,
    [RATE_FTS] = 0.911344,
    [RATE_MS]  = 0.277778,
    [RATE_KMH] = 1,
    [RATE_KTS] = 1.852,
  },

  [RATE_KTS] = {
    [RATE_MPH] = 1.1507794,
    [RATE_FTS] = 1.6878099,
    [RATE_MS]  = 0.5144444,
    [RATE_KMH] = 0.5399568,
    [RATE_KTS] = 1,
  }
};

static ldbl_t (* const temp_coeffs [NUM_TEMPS] [NUM_TEMPS]) (const ldbl_t) = {
  [TEMP_C] = {
    [TEMP_C] = return_1_ldbl_arg,
    [TEMP_K] = convert_temp_C_to_K,
    [TEMP_F] = convert_temp_C_to_F,
    [TEMP_R] = convert_temp_C_to_R,
  },

  [TEMP_K] = {
    [TEMP_C] = convert_temp_K_to_C,
    [TEMP_K] = return_1_ldbl_arg,
    [TEMP_F] = convert_temp_K_to_F,
    [TEMP_R] = convert_temp_K_to_R,
  },

  [TEMP_F] = {
    [TEMP_C] = convert_temp_F_to_C,
    [TEMP_K] = convert_temp_F_to_K,
    [TEMP_F] = return_1_ldbl_arg,
    [TEMP_R] = convert_temp_F_to_R,
  },

  [TEMP_R] = {
    [TEMP_C] = convert_temp_R_to_C,
    [TEMP_K] = convert_temp_R_to_K,
    [TEMP_F] = convert_temp_R_to_F,
    [TEMP_R] = return_1_ldbl_arg,
  }
};

bool compare_eps (const ldbl_t a, const ldbl_t b, const ldbl_t eps) {
  return fabsl(a - b) < eps;
}

pure const_func ldbl_t convert_rate (const ldbl_t x, const rate_t from, const rate_t to) {
  return x * rate_coeffs [from] [to];
}

pure const_func static ldbl_t return_1_ldbl_arg (const ldbl_t a) {
  return a;
}

pure const_func ldbl_t convert_temp (const ldbl_t x, const temp_t from, const temp_t to) {
  return temp_coeffs [from] [to] (x);
}

const_func static int strncasecmp_c (const char* const s1, const char* const s2, const size_t n) {
  const char
    *cs1 = strndup(s1, n),
    *cs2 = strndup(s2, n);

  int cmp = strncasecmp(cs1, cs2, n);

  safefree_args(2, cs1, cs2);

  return cmp;
}

const_func rate_t convert_str_to_rate (const char* const s) {

  static const char* const ratestrs[NUM_RATES][3] = {
    [RATE_MPH] = { "mph" },
    [RATE_FTS] = { "fts" },
    [RATE_MS]  = { "ms" },
    [RATE_KMH] = { "kmh" },
    [RATE_KTS] = { "kts" },
  };

  for (rate_t i = 0; i < NUM_RATES; i++) {
    if ( ! strncasecmp_c(s, (const char* ) ratestrs[i], 3) ) {
      return i;
    }
  }

  return RATE_KMH;
}

char* strip_outerws (const char* const str) {

  if (! str || ! str[0] || ( str_count(str, " ") == safestrnlen(str) )) { return make_empty_str(); }

  const size_t
    bws     = strspn(str, " "),
    restlen = safestrnlen(str + bws);

  const char* const rns = str_reverse(strndup(str + bws, restlen));

  const size_t ews = strspn(rns, " ");

  return str_reverse(strndup(rns + ews, safestrnlen(rns) - ews));
}

  /*
  printf("bws: %zu\nlens: %zu/%zu\n", bws, safestrnlen(str), restlen);
  printf(
    "str + bws:%s:\n"
    "rns      :%s:\n",
    str + bws,
    rns
  );
  printf("ews: %zu\n", ews);
  */


