#include "../Include/string_utilities.h"

void int_to_str(int num, char* str) {
    int i = 0;
    int is_negative = num < 0;
    if(is_negative) num = -num;

    // Convert digits to characters in reverse order
    do {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while(num > 0 && i < STRING_UTILITIES_TYPE_CONVERSION_LENGTH_NUMERIC - 1);

    // Check if this string is too long
    if(i == STRING_UTILITIES_TYPE_CONVERSION_LENGTH_NUMERIC - 1) { // space for neg
        strcpy(str,  STRING_UTILITIES_MSG_TYPE_CONVERSION_TOO_LONG);
        return;
    }

    // Attach negative sign
    if(is_negative) str[i++] = '-';

    // Reverse string
    str[i] = '\0';
    int start = 0, end = i-1;
    while(start < end) {
        char temp = str[start];
        str[start++] = str[end]; // post-increment start
        str[end--] = temp; // post-decerement end
    }

}

void float_to_str(float num, char* str, int digits_after_decimal) {
    int int_part = (int)num;
    float dec_part = num - int_part;

    // Convert integer (with negative)
    int_to_str(int_part, str);

     // Check if this string is too long
    if(strlen(str) == STRING_UTILITIES_TYPE_CONVERSION_LENGTH_NUMERIC - (1 + digits_after_decimal)) { // space for decimal and digits
        strcpy(str,  STRING_UTILITIES_MSG_TYPE_CONVERSION_TOO_LONG);
        return;
    }

    // Add decimal
    int len = strlen(str);
    str[len++] = '.';

    // Convert fraction
    for(int i = 0; i < digits_after_decimal; i++) {
        dec_part *= 10;
        int digit = (int)dec_part;
        str[len++] = '0' + digit;
        dec_part -= digit;
    }

    // Null terminate
    str[len] = '\0';
}

void format_str(char* output, int maxlen, const char *format, ...) {
    va_list args;
    va_start(args, format);

    int i = 0; // output index
    int encountered_error = 0;
    const char *fmt = format; // to iterate through the format string
    
    // Iterate
    while(*fmt != '\0' && i < (maxlen - 1) && !encountered_error) {
        // Check if this is format item
        if(*fmt == '%') {
            fmt++; // Skip over the %, move to the data type
            
            switch(*fmt) {
                 // int %d
                case 'd': {
                    int num = va_arg(args, int), buffer_index = 0;
                    char buffer[STRING_UTILITIES_TYPE_CONVERSION_LENGTH_NUMERIC];
                    int_to_str(num, buffer);
                    while(*(buffer+buffer_index) != '\0' && i < (maxlen - 1)) {
                        output[i++] = *(buffer+(buffer_index++)); // increment buffer_index
                    } 
                    break;
                }
                // string %s
                case 's': { 
                    char* str = va_arg(args, char *);
                    while(*str != '\0' && i < (maxlen - 1)) {
                        output[i++] = *(str++);
                    }
                    break;
                }
                // float %f, default digits after decimal
                case 'f': { 
                    double num = va_arg(args, double);
                    int buffer_index = 0;
                    char buffer[STRING_UTILITIES_TYPE_CONVERSION_LENGTH_NUMERIC];
                    float_to_str(num, buffer, STRING_UTILITIES_FLOAT_DEFAULT_DIGITS_AFTER_DECIMAL);
                    while(*(buffer+buffer_index) != '\0' && i < (maxlen - 1)) {
                        output[i++] = *(buffer+(buffer_index++)); // increment buffer_index
                    } 
                    break;
                }      
                // float %.xf, custom digits after decimal -- 1 <= x <= 9
                case '.': { 
                    fmt++;
                    int digits_after_decimal = (*fmt) - '0';
                    // Enforce between 1 and 9 digits, inclusive
                    if(digits_after_decimal < 1 || digits_after_decimal > 9) {
                        encountered_error = 1;
                        strcpy(output, STRING_UTILITIES_MSG_FLOAT_DIGITS_INVALID);
                        strncpy(output + strlen(STRING_UTILITIES_MSG_FLOAT_DIGITS_INVALID) - 2, fmt, 1);
                        break;
                    } 
                    fmt++; // *fmt should now now "f"
                    if(*fmt != 'f'){
                    // Enforce single digit
                        encountered_error = 1;
                        strcpy(output, STRING_UTILITIES_MSG_FLOAT_DIGITS_TOO_MANY);
                        break;
                    }

                    double num = va_arg(args, double);
                    int buffer_index = 0;
                    char buffer[STRING_UTILITIES_TYPE_CONVERSION_LENGTH_NUMERIC];
                    float_to_str(num, buffer, digits_after_decimal);
                    while(*(buffer+buffer_index) != '\0' && i < (maxlen - 1)) {
                        output[i++] = *(buffer+(buffer_index++)); // increment buffer_index
                    } 
                    break;
                }                              
                // other
                default: {
                    encountered_error = 1;
                    strcpy(output, STRING_UTILITIES_MSG_UNKNOWN_FORMAT_TYPE);
                    
                    // Include the letter that caused the error
                    // This currently does not support multi-letter format strings like %lu
                    strncpy(output + strlen(STRING_UTILITIES_MSG_UNKNOWN_FORMAT_TYPE) - 2, fmt, 1);
                    break;
                }
            }

        } else {
            // Regular character, copy over
            output[i++] = *fmt;
        }

        // Increment the format string
        fmt++;
    }
    va_end(args);

    // Check if we encountered an error
    if(encountered_error) {
        return;
    } else
    // Check if the message is too long
    if(i >= (maxlen - 1)) {
        strcpy(output, STRING_UTILITIES_MSG_TOO_LONG);
        return;
    }

    // Null-terminate the output string
    output[i] = '\0';

}