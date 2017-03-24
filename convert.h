#include "common.h"

typedef long double ldbl_t;

typedef enum {
  RATE_MPH,
  RATE_FTS,
  RATE_MS,
  RATE_KMH,
  RATE_KTS,
  NUM_UNITS
} rate_t;

static const long double rate_coeffs [NUM_UNITS][NUM_UNITS] = {

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
  },
};

ldbl_t convert_rate (const long double x, const rate_t from, const rate_t to) pure const_func;
bool compare_eps (const ldbl_t a, const ldbl_t b, const ldbl_t eps);

bool compare_eps (const ldbl_t a, const ldbl_t b, const ldbl_t eps) {
  return fabsl(a - b) < eps;
}

pure const_func ldbl_t convert_rate (const long double x, const rate_t from, const rate_t to) {
  return x * rate_coeffs [from] [to];
}

#define define_convert_temp_func(from, to, expression) \
  /* static */ ldbl_t convert_temp_ ## from ## _to_ ## to (const ldbl_t value); \
  pure const_func /* warn_unused static */ ldbl_t convert_temp_ ## from ## _to_ ## to \
  (const ldbl_t value) { return (expression); } int IWILLNOTEXIST ## from ## to

define_convert_temp_func(F, C, (5.f/9.f) * (value - 32) );
define_convert_temp_func(C, F, ((9.f/5.f) * value) + 32 );

define_convert_temp_func(K, C, value - 273.15);
define_convert_temp_func(C, K, value + 273.15);

define_convert_temp_func(K, F, convert_temp_C_to_F( convert_temp_K_to_C(value) ) );
define_convert_temp_func(F, K, convert_temp_C_to_K( convert_temp_F_to_C(value) ) );

define_convert_temp_func(R, F, value - 459.69);
define_convert_temp_func(F, R, value + 459.69);
