#include <ctype.h>
#include "strfuncs.h"

void strtoupper(char* s){
  int i = 0;
  while (s[i] != '\0')
    s[i++] = toupper(s[i]);
}
