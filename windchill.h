#include "convert.h"
/*
AT = Ta + (0.33 * e) - (.7 * ws) - 4.0
  Ta = Dry bulb temperature (°C)
  e = Water vapour pressure (hPa)
  ws = Wind speed (m/s) (at an elevation of 10 meters)

e = (rh / 100) * 6.105 * exp( (17.27 * Ta) / (237.7 + Ta))
*/

ldbl_t wc_wvp_from_humtemp (const ldbl_t relhum, const ldbl_t airtemp);
ldbl_t wc_get_windchill (const ldbl_t airtemp_c, const ldbl_t relhum, const ldbl_t windspd, const rate_t wind_unit);

ldbl_t wc_wvp_from_humtemp (const ldbl_t relhum, const ldbl_t airtemp) {
  return (relhum / 100) * 6.105 * expl( (17.27 * airtemp) / (237.7 + airtemp));
}

ldbl_t wc_get_windchill (const ldbl_t airtemp_c, const ldbl_t relhum, const ldbl_t windspd, const rate_t wind_unit) {
  return airtemp_c + (0.33 * wc_wvp_from_humtemp(relhum, airtemp_c)) - (.7 * convert_rate(windspd, wind_unit, RATE_MPH)) - 4.0;
}
