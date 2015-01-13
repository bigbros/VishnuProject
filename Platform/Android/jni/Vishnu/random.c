#include <stdlib.h>

static unsigned long next = 1UL;

void srand(unsigned int seed)
{
#ifdef __H8300__
  int loc = vsns_ini() || sns_loc();
  if (!loc)
    loc_cpu();
#endif
  next = seed;
#ifdef __H8300__
  if (!loc)
    unl_cpu();
#endif
}

int rand(void)
{
#ifdef __H8300__
  int loc = vsns_ini() || sns_loc();
  if (!loc)
    loc_cpu();
#endif
  next = next * 1103515245 + 12345;
  int result = (unsigned int)(next / 65536) % (RAND_MAX+1);
#ifdef __h8300__
  if (!loc)
    unl_cpu();
#endif
  return result;
}

double atof(const char * str)
{
	return strtod(str, 0);
}


