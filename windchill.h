#include <ctype.h>
#include "convert.h"

/*
AT = Ta + (0.33 * e) - (.7 * ws) - 4.0
  Ta = Dry bulb temperature (°C)
  e = Water vapour pressure (hPa)
  ws = Wind speed (m/s) (at an elevation of 10 meters)

e = (rh / 100) * 6.105 * exp( (17.27 * Ta) / (237.7 + Ta))
  Ta = Dry bulb temperature (°C)
  rh = Relative humidity [%]
  exp  represents the exponential function
*/

ldbl_t         wc_wvp_from_humtemp (const ldbl_t relhum, const ldbl_t airtemp, const temp_t temp_unit);
ldbl_t            wc_get_windchill (const ldbl_t airtemp, const temp_t temp_unit, const ldbl_t relhum, const ldbl_t windspd, const rate_t wind_unit);
ldbl_t wc_windchill_from_str_array (char** str, const size_t len);

ldbl_t wc_wvp_from_humtemp (const ldbl_t relhum, const ldbl_t airtemp, const temp_t temp_unit) {
  const ldbl_t abshum = relhum < 0.f // negative?
    ? fabsl(relhum)                  // absolute
    : relhum;                        // nvm

  const ldbl_t adjhum = abshum > 100.f // greater than 100?
    ? 100.f                            // make it 100
    : abshum;                          // nvm

  const ldbl_t c_temp = convert_temp(airtemp, temp_unit, TEMP_C);
  return (adjhum / 100) * 6.105 * expl( (17.27 * c_temp) / (237.7 + c_temp));
}

ldbl_t wc_get_windchill (const ldbl_t airtemp, const temp_t temp_unit, const ldbl_t relhum, const ldbl_t windspd, const rate_t wind_unit) {
  const ldbl_t c_temp = convert_temp(airtemp, temp_unit, TEMP_C);
  return c_temp + (0.33 * wc_wvp_from_humtemp(relhum, c_temp, TEMP_C)) - (.7 * convert_rate(windspd, wind_unit, RATE_MS)) - 4.0;
}

const_func ldbl_t wc_windchill_from_str_array (char** str, const size_t len) {

  if ( len < 5 ) { return 0.f; }

  static const temp_t char2tempunit[] = {
    ['C'] = TEMP_C,
    ['K'] = TEMP_K,
    ['F'] = TEMP_F,
    ['R'] = TEMP_R,
  };

  const temp_t tempunit = char2tempunit[ (unsigned) toupper(str [1] [0]) ];

  const rate_t windunit = convert_str_to_rate(str[3]);

  #define NVALS 3

  char *endptrs[NVALS];

  // non-null
  for (size_t i = 0; i < NVALS; i++) {
    endptrs[i] = safemalloc( sizeof(char) * 20 ); // uninitialised memory
  }

  const ldbl_t vals[NVALS] = {
    strtold(str[0], &endptrs[0]), // temperature
    strtold(str[2], &endptrs[1]), // windspeed
    strtold(str[4], &endptrs[2]), // humidity
  };

  for (size_t i = 0; i < NVALS; i++) {
    if (
      compare_eps(vals[i], 0.f, 1e-8) // vals[i] is 0
      && ( ! strncmp( str[ i == 1 ? 2 : i ], endptrs[i], 20 ) ) // and endptr was unchanged
    ) {
      printf("bad conversion: %s: %s\n", __func__, str[ i == 1 ? 2 : i ] );
      return 0.f;
    }
  }

  return wc_get_windchill(vals[0], tempunit, vals[2], vals[1], windunit);
}
