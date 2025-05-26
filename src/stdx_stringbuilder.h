/*
 * XStrBuilder - Dynamic String Builder for C
 * Part of the STDX General Purpose C Library by marciovmf
 * https://github.com/marciovmf/stdx
 *
 * Provides a simple interface for constructing strings efficiently:
 *   - Dynamic growth as data is appended
 *   - Append strings, characters, substrings, and formatted text
 *   - Convert to null-terminated C string
 *   - Clear or destroy the builder when done
 *
 * To compile the implementation, define:
 *     #define STDX_IMPLEMENTATION_STRINGBUILDER
 * in **one** source file before including this header.
 *
 * Author: marciovmf
 * License: MIT
 * Usage: #include "strbuilder.h"
 */

#ifndef STDX_STRINGBUILDER_H
#define STDX_STRINGBUILDER_H

#ifdef __cplusplus
extern "C"
{
#endif

#define STDX_STRINGBUILDER_VERSION_MAJOR 1
#define STDX_STRINGBUILDER_VERSION_MINOR 0
#define STDX_STRINGBUILDER_VERSION_PATCH 0

#define STDX_STRINGBUILDER_VERSION (STDX_STRINGBUILDER_VERSION_MAJOR * 10000 + STDX_STRINGBUILDER_VERSION_MINOR * 100 + STDX_STRINGBUILDER_VERSION_PATCH)

#ifndef strbuilder_STACK_BUFFER_SIZE
#define strbuilder_STACK_BUFFER_SIZE 255
#endif

  typedef struct XStrBuilder_t XStrBuilder;

  XStrBuilder* strbuilder_create();
  void    strbuilder_append(XStrBuilder *sb, const char *str);
  void    strbuilder_append_char(XStrBuilder* sb, char c);
  void    strbuilder_append_format(XStrBuilder *sb, const char *format, ...);
  void    strbuilder_append_substring(XStrBuilder *sb, const char *start, size_t length);
  char*   strbuilder_to_string(const XStrBuilder *sb);
  void    strbuilder_destroy(XStrBuilder *sb);
  void    strbuilder_clear(XStrBuilder *sb);
  size_t  strbuilder_length(XStrBuilder *sb);

#ifdef STDX_IMPLEMENTATION_STRINGBUILDER

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

  struct XStrBuilder_t
  {
    char *data;
    size_t capacity;
    size_t length;
  };

  XStrBuilder* strbuilder_create()
  {
    XStrBuilder* sb = (XStrBuilder*)  malloc(sizeof(XStrBuilder));
    sb->capacity = 16; // Initial capacity
    sb->data = (char *) malloc(sb->capacity * sizeof(char));
    sb->length = 0;
    sb->data[0] = '\0'; // Null-terminate the string
    return sb;
  }

  void strbuilder_append(XStrBuilder *sb, const char *str)
  {
    size_t strLen = strlen(str);
    size_t newLength = sb->length + strLen;

    if (newLength + 1 > sb->capacity)
    {
      while (newLength + 1 > sb->capacity)
      {
        sb->capacity *= 2; // Double the capacity
      }
      sb->data = (char *)realloc(sb->data, sb->capacity * sizeof(char));
    }

    strcat(sb->data, str);
    sb->length = newLength;
  }

  void strbuilder_append_format(XStrBuilder *sb, const char *format, ...)
  {
    va_list args;
    va_start(args, format);

    // Stack-allocated buffer for small strings
    char stack_buffer[strbuilder_STACK_BUFFER_SIZE];
    static const int STACK_BUFFER_SIZE = strbuilder_STACK_BUFFER_SIZE;

    // Determine the size needed for the formatted string
    size_t needed = vsnprintf(stack_buffer, STACK_BUFFER_SIZE, format, args);

    if (needed < STACK_BUFFER_SIZE)
    {
      // The formatted string fits within the stack buffer
      strbuilder_append(sb, stack_buffer);
    }
    else
    {
      // Allocate memory for the formatted string
      char *formatted_str = (char *)malloc((needed + 1) * sizeof(char));

      // Format the string
      vsnprintf(formatted_str, needed + 1, format, args);

      // Append the formatted string to the builder
      strbuilder_append(sb, formatted_str);

      // Free memory
      free(formatted_str);
    }

    va_end(args);
  }

  void strbuilder_append_char(XStrBuilder* sb, char c)
  {
    char s[2] = {c, 0};
    strbuilder_append(sb, s);
  }

  void strbuilder_append_substring(XStrBuilder *sb, const char *start, size_t length)
  {
    strbuilder_append_format(sb, "%.*s", length, start);
  }

  char* strbuilder_to_string(const XStrBuilder *sb)
  {
    return sb->data;
  }

  void strbuilder_destroy(XStrBuilder *sb)
  {
    free(sb->data);
    sb->data = NULL;
    sb->capacity = 0;
    sb->length = 0;
    free(sb);
  }

  void strbuilder_clear(XStrBuilder *sb)
  {
    if (sb->length > 0)
    {
      sb->data[0] = 0;
      sb->length = 0;
    }
  }

  size_t strbuilder_length(XStrBuilder *sb)
  {
    return sb->length;
  }


#endif // STDX_IMPLEMENTATION_STRINGBUILDER

#ifdef __cplusplus
}
#endif
#endif // STDX_STRINGBUILDER_H

