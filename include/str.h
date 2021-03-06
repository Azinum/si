// str.h

#ifndef _STR_H
#define _STR_H

#include "config.h"

char* string_new_copy(const char* old, int length);

int string_to_number(char* string, double* number);

int safe_string_to_number(char* string, int length, double* number);

void string_free(char* string);

void string_nfree(char* string, int length);

#endif
