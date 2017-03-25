#include "windchill.h"

int main(void) {

  char *in, **spl;

  size_t len;

  while ( true ) {
    in = input_prompt(
      "give temp unit windspd unit hum\n"
      "example: 3.5 C 23.1 kmh 80 \n->",
      100, "no good, retry: "
    );

    if ( safestrnlen(in) ) {

      bool
      ok  = str_count(in, " ") >= 4;
      spl = str_split(in, ' ', &len);
      ok  = len >= 5;

      if (ok) {
        break;
      }

      printf("malformed input: %s\n", in);
    }

    safefree(in);
  }

  ldbl_t wc = wc_windchill_from_str_array(spl, len);

  printf("windchill: %LG\n", wc);

  safefree(spl[0]);
  safefree(spl);

  return 0;
}
