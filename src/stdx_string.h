/*
 * STDX - Lightweight String Utilities
 * Part of the STDX General Purpose C Library by marciovmf
 * https://github.com/marciovmf/stdx
 *
 * This module provides efficient string handling tools for C:
 *   - C string helpers: case-insensitive prefix/suffix matching
 *   - XSmallstr: fixed-capacity, stack-allocated strings
 *   - XStrview: immutable, non-owning views into C strings
 *   - Tokenization and trimming
 *   - UTF-8-aware string length calculation
 *   - Fast substring and search operations
 *   - Case-sensitive and case-insensitive comparisons
 *
 * Useful for environments where dynamic memory is avoided.
 *
 * To compile the implementation, define:
 *     #define STDX_IMPLEMENTATION_STRING
 * in **one** source file before including this header.
 *
 * Author: marciovmf
 * License: MIT
 * Usage: #include "stdx_string.h"
 */
#ifndef STDX_STRING_H
#define STDX_STRING_H

#ifdef __cplusplus
extern "C"
{
#endif

#define STDX_STRING_VERSION_MAJOR 1
#define STDX_STRING_VERSION_MINOR 0
#define STDX_STRING_VERSION_PATCH 0

#define STDX_STRING_VERSION (STDX_STRING_VERSION_MAJOR * 10000 + STDX_STRING_VERSION_MINOR * 100 + STDX_STRING_VERSION_PATCH)

#include <stdbool.h>
#include <stdio.h>

#ifndef STDX_SMALLSTR_MAX_LENGTH
  #define STDX_SMALLSTR_MAX_LENGTH 256
#endif

  typedef struct
  {
    char buf[STDX_SMALLSTR_MAX_LENGTH + 1];
    size_t length;
  } XSmallstr;

  typedef struct
  {
    const XSmallstr* s;
    size_t pos;
    char delimiter;
  } XSmallstrTokenIterator;

  typedef struct
  {
    const char* data;
    size_t length;
  } XStrview;

  // ---------------------------------------------------------------------------
  // C string utilities
  // ---------------------------------------------------------------------------

  char*     x_cstr_str(const char *haystack, const char *needle);
  bool      x_cstr_ends_with(const char* str, const char* suffix);
  bool      x_cstr_starts_with(const char* str, const char* prefix);
  bool      x_cstr_ends_with_ci(const char* str, const char* suffix);
  bool      x_cstr_starts_with_ci(const char* str, const char* prefix);

  // ---------------------------------------------------------------------------
  // XSmallstr
  // ---------------------------------------------------------------------------

  int       x_smallstr_init(XSmallstr* s, const char* str);
  size_t    x_smallstr_format(XSmallstr* s, const char* fmt, ...);
  size_t    x_smallstr_from_cstr(XSmallstr* s, const char* cstr);
  size_t    x_smallstr_from_strview(XStrview sv, XSmallstr* out);
  XStrview  x_smallstr_to_strview(const XSmallstr* s);
  size_t    x_smallstr_length(const XSmallstr* s);
  void      x_smallstr_clear(XSmallstr* s);
  size_t    x_smallstr_append_cstr(XSmallstr* s, const char* cstr);
  size_t    x_smallstr_append_char(XSmallstr* s, char c);
  size_t    x_smallstr_substring(const XSmallstr* s, size_t start, size_t len, XSmallstr* out);
  int       x_smallstr_find(const XSmallstr* s, char c);
  int       x_smallstr_rfind(const XSmallstr* s, char c);
  int       x_smallstr_split_at(const XSmallstr* s, char delim, XSmallstr* left, XSmallstr* right);
  int       x_smallstr_next_token(XSmallstr* input, char delim, XSmallstr* token);
  void      x_smallstr_trim_left(XSmallstr* s);
  void      x_smallstr_trim_right(XSmallstr* s);
  void      x_smallstr_trim(XSmallstr* s);
  int       x_smallstr_compare_case_insensitive(const XSmallstr* a, const XSmallstr* b);
  int       x_smallstr_replace_all(XSmallstr* s, const char* find, const char* replace);
  void      x_smallstr_token_iter_init(XSmallstrTokenIterator* iter, const XSmallstr* s, char delimiter);
  int       x_smallstr_token_iter_next(XSmallstrTokenIterator* iter, XSmallstr* token);
  size_t    x_smallstr_utf8_len(const XSmallstr* s);
  int       x_smallstr_cmp(const XSmallstr* a, const XSmallstr* b);
  int       x_smallstr_cmp_cstr(const XSmallstr* a, const char* b);
  const char* x_smallstr_cstr(const XSmallstr* s);

  // ---------------------------------------------------------------------------
  // Stringview
  // ---------------------------------------------------------------------------

#define x_strview(cstr) ((XStrview){ .data = (cstr), .length = strlen(cstr) })

  bool      x_strview_empty(XStrview sv);
  bool      x_strview_eq(XStrview a, XStrview b);
  bool      x_strview_eq_cstr(XStrview a, const char* b);
  int       x_strview_cmp(XStrview a, XStrview b);
  bool      x_strview_case_eq(XStrview a, XStrview b);
  int       x_strview_case_cmp(XStrview a, XStrview b);
  XStrview  x_strview_substr(XStrview sv, size_t start, size_t len);
  XStrview  x_strview_trim_left(XStrview sv);
  XStrview  x_strview_trim_right(XStrview sv);
  XStrview  x_strview_trim(XStrview sv);
  int       x_strview_find(XStrview sv, char c);
  int       x_strview_rfind(XStrview sv, char c);
  bool      x_strview_split_at(XStrview sv, char delim, XStrview* left, XStrview* right);
  bool      x_strview_next_token(XStrview* input, char delim, XStrview* token);

#ifdef STDX_IMPLEMENTATION_STRING

#include <ctype.h>   // tolower
#include <stddef.h>  // size_t
#include <string.h>  // strlen, memcmp, strncasecmp (POSIX)
#include <ctype.h>   // tolower
#include <stdint.h>

#ifdef _WIN32
  #define strncasecmp _strnicmp
#endif

  char* x_cstr_str(const char *haystack, const char *needle)
  {
    if (!*needle)
      return (char *)haystack;

    for (; *haystack; haystack++) {
      const char *h = haystack;
      const char *n = needle;

      while (*h && *n && tolower((unsigned char)*h) == tolower((unsigned char)*n))
      {
        h++;
        n++;
      }

      if (!*n)  // Reached end of needle => match
        return (char *)haystack;
    }
    return NULL;
  }

  bool x_cstr_ends_with(const char* str, const char* suffix)
  {
    const char* found = strstr(str, suffix);
    if (found == NULL || found + strlen(suffix) != str + strlen(str))

    {
      return false;
    }
    return true;
  }

  bool x_cstr_starts_with(const char* str, const char* prefix)
  {
    const char* found = strstr(str, prefix);
    bool match = (found == str);
    if (match && prefix[0] == 0 && str[0] != 0)
      match = false;

    return match;
  }

  bool x_cstr_starts_with_ci(const char* str, const char* prefix)
  {
    const char* found = x_cstr_str(str, prefix);
    bool match = (found == str);
    if (match && prefix[0] == 0 && str[0] != 0)
      match = false;

    return match;
  }

  bool x_cstr_ends_with_ci(const char* str, const char* suffix)
  {
    const char* found = x_cstr_str(str, suffix);
    if (found == NULL || found + strlen(suffix) != str + strlen(str))

    {
      return false;
    }
    return true;
  }

  unsigned int str_hash(const char* str)
  {
    unsigned int hash = 2166136261u; // FNV offset basis
    while (*str)

    {
      hash ^= (unsigned int) *str++;
      hash *= 16777619u; // FNV prime
    }
    return hash;
  }

  size_t smallstr(XSmallstr* smallString, const char* str)
  {
    size_t length = strlen(str);
#if defined(_DEBUG) || defined(DEBUG)
#if defined(STDX_x_smallstr_ENABLE_DBG_MESSAGES)
    if (length >= STDX_SMALLSTR_MAX_LENGTH)

    {
      log_print_debug("The string length (%d) exceeds the maximum size of a XSmallstr (%d)", length, STDX_SMALLSTR_MAX_LENGTH);
    }
#endif
#endif
    strncpy((char *) &smallString->buf, str, STDX_SMALLSTR_MAX_LENGTH - 1);
    smallString->buf[STDX_SMALLSTR_MAX_LENGTH - 1] = 0;
    smallString->length = strlen(smallString->buf);
    return smallString->length;
  }

  size_t x_smallstr_format(XSmallstr* smallString, const char* fmt, ...)
  {
    va_list argList;
    va_start(argList, fmt);
    smallString->length = vsnprintf((char*) &smallString->buf, STDX_SMALLSTR_MAX_LENGTH-1, fmt, argList);
    va_end(argList);

    bool success = smallString->length > 0 && smallString->length < STDX_SMALLSTR_MAX_LENGTH;
    return success;
  }

  size_t x_smallstr_from_cstr(XSmallstr* s, const char* cstr)
  {
    size_t len = strlen(cstr);
    if (len > STDX_SMALLSTR_MAX_LENGTH)
      return -1;
    memcpy(s->buf, cstr, len);
    s->buf[len] = '\0';
    s->length = len;
    return len;
  }

  const char* x_smallstr_cstr(const XSmallstr* s)
  {
    ((char*)s->buf)[s->length] = '\0';
    return (const char*)s->buf;
  }

  size_t x_smallstr_length(const XSmallstr* smallString)
  {
    return smallString->length;
  }

  void x_smallstr_clear(XSmallstr* smallString)
  {
    memset(smallString->buf, 0, STDX_SMALLSTR_MAX_LENGTH * sizeof(char));
    smallString->length = 0;
  }

  size_t x_smallstr_append_cstr(XSmallstr* s, const char* cstr)
  {
    size_t len = strlen(cstr);
    if (s->length + len > STDX_SMALLSTR_MAX_LENGTH) return -1;
    memcpy(&s->buf[s->length], cstr, len);
    s->length += len;
    s->buf[s->length] = '\0';
    return s->length;
  }

  size_t x_smallstr_append_char(XSmallstr* s, char c)
  {
    if (s->length + 1 > STDX_SMALLSTR_MAX_LENGTH) return -1;
    s->buf[s->length++] = (uint8_t)c;
    s->buf[s->length] = '\0';
    return s->length;
  }

  size_t x_smallstr_substring(const XSmallstr* s, size_t start, size_t len, XSmallstr* out)
  {
    if (start > s->length || start + len > s->length) return -1;
    out->length = len;
    memcpy(out->buf, &s->buf[start], len);
    out->buf[len] = '\0';
    return len;
  }

  void x_smallstr_trim_left(XSmallstr* s)
  {
    size_t i = 0;
    while (i < s->length && isspace(s->buf[i])) i++;
    if (i > 0)
    {
      memmove(s->buf, &s->buf[i], s->length - i);
      s->length -= i;
      s->buf[s->length] = '\0';
    }
  }

  void x_smallstr_trim_right(XSmallstr* s)
  {
    while (s->length > 0 && isspace(s->buf[s->length - 1]))
    {
      s->length--;
    }
    s->buf[s->length] = '\0';
  }

  void x_smallstr_trim(XSmallstr* s)
  {
    x_smallstr_trim_right(s);
    x_smallstr_trim_left(s);
  }

  int x_smallstr_compare_case_insensitive(const XSmallstr* a, const XSmallstr* b)
  {
    if (a->length != b->length) return 0;
    for (size_t i = 0; i < a->length; i++)
    {
      if (tolower(a->buf[i]) != tolower(b->buf[i])) return 0;
    }
    return 1;
  }

  int x_smallstr_replace_all(XSmallstr* s, const char* find, const char* replace)
  {
    XSmallstr result;
    x_smallstr_clear(&result);

    size_t find_len = strlen(find);
    size_t replace_len = strlen(replace);
    size_t i = 0;

    while (i < s->length)
    {
      if (i + find_len <= s->length && memcmp(&s->buf[i], find, find_len) == 0)
      {
        if (result.length + replace_len > STDX_SMALLSTR_MAX_LENGTH) return -1;
        memcpy(&result.buf[result.length], replace, replace_len);
        result.length += replace_len;
        i += find_len;
      } else
      {
        if (result.length + 1 > STDX_SMALLSTR_MAX_LENGTH) return -1;
        result.buf[result.length++] = s->buf[i++];
      }
    }

    result.buf[result.length] = '\0';
    *s = result;
    return 0;
  }

  void x_smallstr_token_iter_init(XSmallstrTokenIterator* iter, const XSmallstr* s, char delimiter)
  {
    iter->s = s;
    iter->pos = 0;
    iter->delimiter = delimiter;
  }

  int x_smallstr_token_iter_next(XSmallstrTokenIterator* iter, XSmallstr* token)
  {
    if (iter->pos >= iter->s->length) return 0;

    size_t start = iter->pos;
    while (iter->pos < iter->s->length && iter->s->buf[iter->pos] != (uint8_t)iter->delimiter)
    {
      iter->pos++;
    }

    x_smallstr_substring(iter->s, start, iter->pos - start, token);

    if (iter->pos < iter->s->length) iter->pos++; // Skip delimiter

    return 1;
  }

  size_t x_smallstr_utf8_len(const XSmallstr* s)
  {
    size_t count = 0;
    for (size_t i = 0; i < s->length;)
    {
      uint8_t c = s->buf[i];
      if ((c & 0x80) == 0x00) i += 1;
      else if ((c & 0xE0) == 0xC0) i += 2;
      else if ((c & 0xF0) == 0xE0) i += 3;
      else if ((c & 0xF8) == 0xF0) i += 4;
      else return count; // invalid byte
      count++;
    }
    return count;
  }

  size_t x_smallstr_from_strview(XStrview sv, XSmallstr* out)
  {
    x_smallstr_clear(out);
    if (sv.length > STDX_SMALLSTR_MAX_LENGTH) return -1;
    memcpy(out->buf, sv.data, sv.length);
    out->buf[sv.length] = '\0';
    out->length = sv.length;
    return out->length;
  }

  int x_smallstr_cmp(const XSmallstr* a, const XSmallstr* b)
  {
    return strncmp(a->buf, b->buf, b->length);
  }

  int x_smallstr_cmp_cstr(const XSmallstr* a, const char* b)
  {
    return strncmp(a->buf, b, a->length);
  }

  int x_smallstr_find(const XSmallstr* s, char c)
  {
    for (size_t i = 0; i < s->length; ++i)
    {
      if (s->buf[i] == c) return (int)i;
    }
    return -1;
  }

  int x_smallstr_rfind(const XSmallstr* s, char c)
  {
    for (size_t i = s->length; i > 0; --i)
    {
      if (s->buf[i - 1] == c) return (int)(i - 1);
    }
    return -1;
  }

  int x_smallstr_split_at(const XSmallstr* s, char delim, XSmallstr* left, XSmallstr* right)
  {
    int pos = x_smallstr_find(s, delim);
    if (pos < 0) return 0;
    x_smallstr_substring(s, 0, pos, left);
    x_smallstr_substring(s, pos + 1, s->length - pos - 1, right);
    return 1;
  }

  int x_smallstr_next_token(XSmallstr* input, char delim, XSmallstr* token)
  {
    XSmallstr temp;
    int found = 0;
    int pos = x_smallstr_find(input, delim);
    if (pos >= 0)
    {
      x_smallstr_substring(input, 0, pos, token);
      x_smallstr_substring(input, pos + 1, input->length - pos - 1, &temp);
      *input = temp;
      found = 1;
    }
    else if (input->length > 0)
    {
      *token = *input;
      x_smallstr_clear(input);
      found = 1;
    }
    return found;
  }

  XStrview x_smallstr_to_strview(const XSmallstr* s)
  {
    return (XStrview){ s->buf, s->length };
  }

  inline bool x_strview_empty(XStrview sv)
  {
    return sv.length == 0;
  }

  bool x_strview_eq(XStrview a, XStrview b)
  {
    return a.length == b.length && (memcmp(a.data, b.data, a.length) == 0);
  }

  bool x_strview_eq_cstr(XStrview a, const char* b)
  {
    return x_strview_eq(a, x_strview(b));
  }

  int x_strview_cmp(XStrview a, XStrview b)
  {
    size_t min_len = a.length < b.length ? a.length : b.length;
    int r = memcmp(a.data, b.data, min_len);
    if (r != 0) return r;
    return (int)(a.length - b.length);
  }

  bool x_strview_case_eq(XStrview a, XStrview b)
  {
    if (a.length != b.length) return 0;
    for (size_t i = 0; i < a.length; i++)

    {
      if (tolower((unsigned char)a.data[i]) != tolower((unsigned char)b.data[i]))
        return false;
    }
    return true;
  }

  int x_strview_case_cmp(XStrview a, XStrview b)
  {
    size_t min_len = a.length < b.length ? a.length : b.length;
    for (size_t i = 0; i < min_len; i++)

    {
      int ca = tolower((unsigned char)a.data[i]);
      int cb = tolower((unsigned char)b.data[i]);
      if (ca != cb) return ca - cb;
    }
    return (int)(a.length - b.length);
  }

  XStrview x_strview_substr(XStrview sv, size_t start, size_t len)
  {
    if (start > sv.length) start = sv.length;
    if (start + len > sv.length) len = sv.length - start;
    return (XStrview){ sv.data + start, len };
  }

  XStrview x_strview_trim_left(XStrview sv)
  {
    size_t i = 0;
    while (i < sv.length && (unsigned char)sv.data[i] <= ' ') i++;
    return x_strview_substr(sv, i, sv.length - i);
  }

  XStrview x_strview_trim_right(XStrview sv)
  {
    size_t i = sv.length;
    while (i > 0 && (unsigned char)sv.data[i - 1] <= ' ') i--;
    return x_strview_substr(sv, 0, i);
  }

  XStrview x_strview_trim(XStrview sv)
  {
    return x_strview_trim_right(x_strview_trim_left(sv));
  }

  int x_strview_find(XStrview sv, char c)
  {
    for (size_t i = 0; i < sv.length; i++)
    {
      if (sv.data[i] == c) return (int)i;
    }
    return -1;
  }

  int x_strview_rfind(XStrview sv, char c)
  {
    for (size_t i = sv.length; i > 0; i--)
    {
      if (sv.data[i - 1] == c) return (int)(i - 1);
    }
    return -1;
  }

  bool x_strview_split_at(XStrview sv, char delim, XStrview* left, XStrview* right)
  {
    int pos = x_strview_find(sv, delim);
    if (pos < 0) return false;
    *left = x_strview_substr(sv, 0, pos);
    *right = x_strview_substr(sv, pos + 1, sv.length - pos - 1);
    return true;
  }

  // Yields the next token before `delim` and advances input
  bool x_strview_next_token(XStrview* input, char delim, XStrview* token)
  {
    XStrview rest;
    if (x_strview_split_at(*input, delim, token, &rest))
    {
      *input = rest;
      return true;
    }
    else if (input->length > 0)
    {
      *token = *input;
      *input = (XStrview){0};
      return 1;
    }
    return false;
  }

#endif // STDX_IMPLEMENTATION_STRING

#ifdef __cplusplus
}
#endif

#endif // STDX_STRING_H

