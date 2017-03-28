#include "windchill.h"

int main(void) {

  char** spl;

  size_t len;

  while ( true ) {
    char* in = input_prompt(
      "give temp unit windspd unit hum\n"
      "example: 3.5 C 23.1 kmh 80 \n->",
      100, "no good, retry: "
    );
    char* cut = strip_outerws(in);
    safefree(in);

    if ( safestrnlen(cut) ) {

      bool ok  = str_count(cut, " ") >= 4;

      spl = str_split(cut, ' ', &len);
      ok  = len >= 5;

      if (ok) {
        safefree(cut);
        break;
      }

      printf("malformed input: %s\n", cut);
      
    }

    safefree(cut);
  }

  ldbl_t wc = wc_windchill_from_str_array(spl, len);

  printf("windchill: %LG\n", wc);

  safefree(spl[0]);
  safefree(spl);

  return 0;
}
