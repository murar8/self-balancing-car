#include "convert.h"
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>

double stringToDouble(char *s)
{
    char *endptr;
    errno = 0;

    double result = strtod(s, &endptr);

    if (s == endptr || errno != 0)
    {
        return NAN;
    }

    for (; isspace((unsigned char)*endptr); endptr++)
        ;

    if (*endptr)
    {
        return NAN;
    }

    return result;
}
