/*
  Copyright (c) 2009-2017 Dave Gamble and cJSON contributors

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unity/examples/unity_config.h"
#include "unity/src/unity.h"
#include "common.h"

static cJSON item[1];

static void assert_is_value(cJSON *value_item, int type)
{
    TEST_ASSERT_NOT_NULL_MESSAGE(value_item, "Item is NULL.");

    assert_not_in_list(value_item);
    assert_has_type(value_item, type);
    assert_has_no_reference(value_item);
    assert_has_no_const_string(value_item);
    assert_has_no_string(value_item);
}

static void assert_parse_value(const char *string, int type)
{
    parse_buffer buffer = { 0, 0, 0, 0, { 0, 0, 0 } };
    buffer.content = (const unsigned char*) string;
    buffer.length = strlen(string) + sizeof("");
    buffer.hooks = global_hooks;

    TEST_ASSERT_TRUE(parse_value(item, &buffer));
    assert_is_value(item, type);
}

static void assert_parse_value_failure(const char *string)
{
    parse_buffer buffer = { 0, 0, 0, 0, { 0, 0, 0 } };
    buffer.content = (const unsigned char*) string;
    buffer.length = strlen(string) + sizeof("");
    buffer.hooks = global_hooks;

    TEST_ASSERT_FALSE(parse_value(item, &buffer));
}

static void parse_value_should_parse_null(void)
{
    assert_parse_value("null", cJSON_NULL);
    reset(item);
}

static void parse_value_should_parse_true(void)
{
    assert_parse_value("true", cJSON_True);
    reset(item);
}

static void parse_value_should_parse_false(void)
{
    assert_parse_value("false", cJSON_False);
    reset(item);
}

static void parse_value_should_parse_number(void)
{
    assert_parse_value("1.5", cJSON_Number);
    reset(item);
}

static void parse_value_should_parse_string(void)
{
    assert_parse_value("\"\"", cJSON_String);
    reset(item);
    assert_parse_value("\"hello\"", cJSON_String);
    reset(item);
}

static void parse_value_should_parse_array(void)
{
    assert_parse_value("[]", cJSON_Array);
    reset(item);
}

static void parse_value_should_parse_object(void)
{
    assert_parse_value("{}", cJSON_Object);
    reset(item);
}

static void parse_value_should_reject_empty_input(void)
{
    assert_parse_value_failure("");
    reset(item);
}

static void parse_value_should_reject_unexpected_terminators(void)
{
    assert_parse_value_failure("nullx");
    reset(item);
    assert_parse_value_failure("truefalse");
    reset(item);
    assert_parse_value_failure("1.5abc");
    reset(item);
    assert_parse_value_failure("\"unterminated");
    reset(item);
}

static void parse_value_should_reject_malformed_json(void)
{
    assert_parse_value_failure("{");
    reset(item);
    assert_parse_value_failure("[");
    reset(item);
    assert_parse_value_failure("{]");
    reset(item);
    assert_parse_value_failure("[}");
    reset(item);
    assert_parse_value_failure("{\"a\":");
    reset(item);
    assert_parse_value_failure("[1,]");
    reset(item);
}

static void parse_value_should_reject_extremely_large_numbers(void)
{
    assert_parse_value_failure("1e99999999999999999999999999999999999999999999999999");
    reset(item);
    assert_parse_value_failure("-1e99999999999999999999999999999999999999999999999999");
    reset(item);
}

static void parse_value_should_reject_deep_nesting(void)
{
    char deep_array[2048];
    char deep_object[2048];
    size_t i;

    for(i = 0; i < sizeof(deep_array) - 2; i++)
    {
        deep_array[i] = '[';
        deep_object[i] = '{';
    }
    deep_array[sizeof(deep_array) - 2] = ']';
    deep_array[sizeof(deep_array) - 1] = '\0';
    deep_object[sizeof(deep_object) - 2] = '}';
    deep_object[sizeof(deep_object) - 1] = '\0';

    assert_parse_value_failure(deep_array);
    reset(item);
    assert_parse_value_failure(deep_object);
    reset(item);
}

int CJSON_CDECL main(void)
{
    /* initialize cJSON item */
    memset(item, 0, sizeof(cJSON));
    UNITY_BEGIN();
    RUN_TEST(parse_value_should_parse_null);
    RUN_TEST(parse_value_should_parse_true);
    RUN_TEST(parse_value_should_parse_false);
    RUN_TEST(parse_value_should_parse_number);
    RUN_TEST(parse_value_should_parse_string);
    RUN_TEST(parse_value_should_parse_array);
    RUN_TEST(parse_value_should_parse_object);
    RUN_TEST(parse_value_should_reject_empty_input);
    RUN_TEST(parse_value_should_reject_unexpected_terminators);
    RUN_TEST(parse_value_should_reject_malformed_json);
    RUN_TEST(parse_value_should_reject_extremely_large_numbers);
    RUN_TEST(parse_value_should_reject_deep_nesting);
    return UNITY_END();
}
