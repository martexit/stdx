#define STDX_IMPLEMENTATION_TEST
#include <stdx_test.h>
#define STDX_IMPLEMENTATION_STRINGBUILDER
#include <stdx_stringbuilder.h>

int test_strbuilder_append_and_to_string()
{
  XStrBuilder* sb = strbuilder_create();
  ASSERT_TRUE(sb != NULL);

  strbuilder_append(sb, "Hello");
  strbuilder_append(sb, ", ");
  strbuilder_append(sb, "World!");

  const char* result = strbuilder_to_string(sb);
  ASSERT_TRUE(result != NULL);
  ASSERT_EQ(strcmp(result, "Hello, World!"), 0);

  strbuilder_destroy(sb);
  return 0;
}

int test_strbuilder_append_char()
{
  XStrBuilder* sb = strbuilder_create();
  ASSERT_TRUE(sb != NULL);

  strbuilder_append_char(sb, 'A');
  strbuilder_append_char(sb, 'B');
  strbuilder_append_char(sb, 'C');

  const char* result = strbuilder_to_string(sb);
  ASSERT_EQ(strcmp(result, "ABC"), 0);

  strbuilder_destroy(sb);
  return 0;
}

int test_strbuilder_append_format()
{
  XStrBuilder* sb = strbuilder_create();
  ASSERT_TRUE(sb != NULL);

  strbuilder_append_format(sb, "%d + %d = %d", 2, 3, 5);

  const char* result = strbuilder_to_string(sb);
  ASSERT_EQ(strcmp(result, "2 + 3 = 5"), 0);

  strbuilder_destroy(sb);
  return 0;
}

int test_strbuilder_append_substring()
{
  XStrBuilder* sb = strbuilder_create();
  ASSERT_TRUE(sb != NULL);

  const char* text = "substring test";
  strbuilder_append_substring(sb, text, 9);  // "substring"

  const char* result = strbuilder_to_string(sb);
  ASSERT_EQ(strcmp(result, "substring"), 0);

  strbuilder_destroy(sb);
  return 0;
}

int test_strbuilder_clear_and_length()
{
  XStrBuilder* sb = strbuilder_create();
  ASSERT_TRUE(sb != NULL);

  strbuilder_append(sb, "temp");
  ASSERT_EQ(strbuilder_length(sb), 4);

  strbuilder_clear(sb);
  ASSERT_EQ(strbuilder_length(sb), 0);

  const char* result = strbuilder_to_string(sb);
  ASSERT_EQ(strcmp(result, ""), 0);

  strbuilder_destroy(sb);
  return 0;
}

int main()
{
  STDXTestCase tests[] = {
    TEST_CASE(test_strbuilder_append_and_to_string),
    TEST_CASE(test_strbuilder_append_char),
    TEST_CASE(test_strbuilder_append_format),
    TEST_CASE(test_strbuilder_append_substring),
    TEST_CASE(test_strbuilder_clear_and_length),
  };

  return stdx_run_tests(tests, sizeof(tests) / sizeof(tests[0]));

  return stdx_run_tests(tests, sizeof(tests)/sizeof(tests[0]));
}
