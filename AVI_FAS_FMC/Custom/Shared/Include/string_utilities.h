#ifndef STRING_UTILITIES_H
#define STRING_UTILITIES_H

#include <string.h>
#include <stdarg.h>

/* stdlib and stdio are costly to use, so we implement relevant utilities */

#define MAX_STRING_LENGTH 1024
#define STRING_UTILITIES_MSG_TOO_LONG ("String too long\n\0")
#define STRING_UTILITIES_MSG_UNKNOWN_FORMAT_TYPE ("Unknown format type: ?\n\0")
#define STRING_UTILITIES_MSG_TYPE_CONVERSION_TOO_LONG ("String conversion too long\n\0")
#define STRING_UTILITIES_MSG_FLOAT_DIGITS_INVALID ("Invalid float digits: ?\n\0")
#define STRING_UTILITIES_MSG_FLOAT_DIGITS_TOO_MANY ("Invalid float digits, max one character\n\0")
#define STRING_UTILITIES_TYPE_CONVERSION_LENGTH_NUMERIC 32 // assume no numbers are more than this many characters long
#define STRING_UTILITIES_FLOAT_DEFAULT_DIGITS_AFTER_DECIMAL 4 // assume no numbers are more than this many characters long

/* Custom function, replacing itoa */
void int_to_str(int num, char* str);

/* Custom function, replacing ftoa */
void float_to_str(float num, char* str, int digits_after_decimal);

/* Custom implementation of sprintf */
void format_str(char* output, int maxlen, const char *format, ...);

#endif /* STRING_UTILITIES_H */