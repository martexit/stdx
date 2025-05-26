#include "stdx_common.h"
#define STDX_IMPLEMENTATION_TEST
#include <stdx_test.h>
#define STDX_IMPLEMENTATION_STRING
#include <stdx_string.h>
#include <stdx_log.h>

int test_str_starts_with(void)
{
  ASSERT_TRUE(x_cstr_starts_with("hello world", "hello"));
  ASSERT_FALSE(x_cstr_starts_with("hello world", "world"));
  ASSERT_TRUE(x_cstr_starts_with("", ""));
  ASSERT_FALSE(x_cstr_starts_with("abc", ""));
  return STDX_TEST_SUCCESS;
}

int test_str_ends_with(void)
{
  ASSERT_TRUE(x_cstr_ends_with("hello world", "world"));
  ASSERT_FALSE(x_cstr_ends_with("hello world", "hello"));
  ASSERT_TRUE(x_cstr_ends_with("", ""));
  ASSERT_FALSE(x_cstr_ends_with("abc", ""));
  return STDX_TEST_SUCCESS;
}

int test_x_smallstr_basic(void)
{
  XSmallstr s;
  size_t len = smallstr(&s, "test123");
  ASSERT_EQ(len, 7);
  ASSERT_EQ(x_smallstr_length(&s), 7);
  return STDX_TEST_SUCCESS;
}

int test_x_smallstr_truncation(void)
{
  XSmallstr s;
  char long_str[1024] = {0};
  memset(long_str, 'a', sizeof(long_str) - 1);
  long_str[sizeof(long_str) - 1] = '\0';
  size_t len = smallstr(&s, long_str);
  ASSERT_TRUE(len <= STDX_SMALLSTR_MAX_LENGTH);
  ASSERT_EQ(x_smallstr_length(&s), len);
  return STDX_TEST_SUCCESS;
}

int test_x_smallstr_format(void)
{
  XSmallstr s;
  size_t len = x_smallstr_format(&s, "val: %d", 42);
  ASSERT_TRUE(strncmp(x_smallstr_cstr(&s), "val: 42", 7) == 0);
  ASSERT_TRUE(len > 0);
  ASSERT_TRUE(x_cstr_starts_with(s.buf, "val: 42"));
  return STDX_TEST_SUCCESS;
}

int test_x_smallstr_clear(void)
{
  XSmallstr s;
  smallstr(&s, "clear me");
  x_smallstr_clear(&s);
  ASSERT_EQ(x_smallstr_length(&s), 0);
  return STDX_TEST_SUCCESS;
}

int test_str_hash(void)
{
  u32 hash1 = str_hash("test");
  u32 hash2 = str_hash("test");
  u32 hash3 = str_hash("different");
  ASSERT_EQ(hash1, hash2);
  ASSERT_NEQ(hash1, hash3);
  return STDX_TEST_SUCCESS;
}

int test_x_strview_empty(void)
{
  ASSERT_TRUE(x_strview_empty(x_strview("")));
  ASSERT_FALSE(x_strview_empty(x_strview("a")));
  return 0;
}

int test_x_strview_eq_and_cmp(void)
{
  XStrview a = x_strview("hello");
  XStrview b = x_strview("hello");
  XStrview c = x_strview("world");

  ASSERT_TRUE(x_strview_eq(a, b));
  ASSERT_FALSE(x_strview_eq(a, c));
  ASSERT_TRUE(x_strview_cmp(a, b) == 0);
  ASSERT_TRUE(x_strview_cmp(a, c) < 0);
  ASSERT_TRUE(x_strview_cmp(c, a) > 0);
  return 0;
}

int test_x_strview_case_eq_and_cmp(void)
{
  ASSERT_TRUE(x_strview_case_eq(x_strview("HELLO"), x_strview("hello")));
  ASSERT_TRUE(x_strview_case_cmp(x_strview("HELLO"), x_strview("hello"))  == 0);
  ASSERT_TRUE(x_strview_case_cmp(x_strview("abc"), x_strview("DEF"))      < 0);
  return 0;
}

int test_x_strview_substr(void)
{
  XStrview sv = x_strview("abcdef");
  ASSERT_TRUE(x_strview_eq_cstr(x_strview_substr(sv, 0, 3), "abc"));
  ASSERT_TRUE(x_strview_eq_cstr(x_strview_substr(sv, 2, 2), "cd"));
  ASSERT_TRUE(x_strview_eq_cstr(x_strview_substr(sv, 4, 10), "ef")); // len clipped
  return 0;
}

int test_x_strview_trim(void)
{
  ASSERT_TRUE(x_strview_eq_cstr(x_strview_trim_left(x_strview("   abc")), "abc"));
  ASSERT_TRUE(x_strview_eq_cstr(x_strview_trim_right(x_strview("abc   ")), "abc"));
  ASSERT_TRUE(x_strview_eq_cstr(x_strview_trim(x_strview("   abc   ")), "abc"));
  ASSERT_TRUE(x_strview_eq_cstr(x_strview_trim(x_strview("abc")), "abc"));
  ASSERT_TRUE(x_strview_eq_cstr(x_strview_trim(x_strview("   ")), ""));
  return 0;
}

int test_x_strview_find_and_rfind(void)
{
  XStrview sv = x_strview("abacada");

  ASSERT_TRUE(x_strview_find(sv, 'a') == 0);
  ASSERT_TRUE(x_strview_find(sv, 'c') == 3);
  ASSERT_TRUE(x_strview_find(sv, 'x') == -1);

  ASSERT_TRUE(x_strview_rfind(sv, 'a') == 6);
  ASSERT_TRUE(x_strview_rfind(sv, 'b') == 1);
  ASSERT_TRUE(x_strview_rfind(sv, 'x') == -1);
  return 0;
}

int test_x_strview_split_at(void)
{

  { // Basic split test
    XStrview sv = x_strview("key:value");
    XStrview left, right;
    ASSERT_TRUE(x_strview_split_at(sv, ':', &left, &right));
    ASSERT_TRUE(x_strview_eq_cstr(left, "key"));
    ASSERT_TRUE(x_strview_eq_cstr(right, "value"));
  }

  { // Test advancing the stringview by tokens
    const char* results[] = { "wako", "yako", "dotty" };
    XStrview csv = x_strview("wako,yako,dotty");
    XStrview token;
    int i = 0;
    while (x_strview_next_token(&csv, ',', &token))
    {
      ASSERT_TRUE(x_strview_eq(x_strview(results[i]), token) == 1);
      i++;
    }
  }

  { // Test splitting by non existent separator
    XStrview sv = x_strview("novalue");
    XStrview left, right;
    ASSERT_FALSE(x_strview_split_at(sv, ':', &left, &right));
  }
  return 0;
}


int main()
{
  STDXTestCase tests[] = {
    TEST_CASE(test_str_starts_with),
    TEST_CASE(test_str_ends_with),
    TEST_CASE(test_x_smallstr_basic),
    TEST_CASE(test_x_smallstr_truncation),
    TEST_CASE(test_x_smallstr_format),
    TEST_CASE(test_x_smallstr_clear),
    TEST_CASE(test_str_hash),

    TEST_CASE(test_x_strview_empty),
    TEST_CASE(test_x_strview_eq_and_cmp),
    TEST_CASE(test_x_strview_case_eq_and_cmp),
    TEST_CASE(test_x_strview_substr),
    TEST_CASE(test_x_strview_trim),
    TEST_CASE(test_x_strview_find_and_rfind),
    TEST_CASE(test_x_strview_split_at),
  };

  return stdx_run_tests(tests, sizeof(tests)/sizeof(tests[0]));
}
