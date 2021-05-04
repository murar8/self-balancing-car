/// Collection of utility functions for converting between data types.

#ifndef _CONVERT_H_
#define _CONVERT_H_

/// Convert a null terminated string to a floating point value.
/// Returns NaN if an error occured during the conversion.
double stringToDouble(char *str);

#endif
