/*
The MIT License

Copyright (c) 2008 jacob berkman <jacob@ilovegom.org>

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
#include "config.h"

#include "checkgomvalue.h"
#include "checkgom.h"

#include <gom/gomvalue.h>
#include <jsapi.h>

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

static GError *err;
static GValue gval = { 0 };
static jsval  jval;

#define JSINT_VALUES                            \
    JSVAL_TO_INT (JSVAL_INT_MIN)),              \
        JSVAL_TO_INT (JSVAL_INT_MIN / 2)),      \
        -3700),                                 \
        -270),                                  \
        -1),                                    \
        0),                                     \
        1),                                     \
        270),                                   \
        3700),                                  \
        JSVAL_TO_INT (JSVAL_INT_MAX / 2)),      \
        JSVAL_TO_INT (JSVAL_INT_MAX)

#define JSDOUBLE_VALUES                         \
    G_E, -G_E),                                 \
        G_LN2, -G_LN2),                         \
        G_LN10, -G_LN10),                       \
        G_PI, -G_PI),                           \
        G_PI_2, -G_PI_2),                       \
        G_PI_4, -G_PI_4),                       \
        G_SQRT2, -G_SQRT2

#define GINT_VALUES                             \
    INT_MIN),                                   \
        INT_MIN / 2),                           \
        INT_MAX / 2),                           \
        INT_MAX

#define GUINT_VALUES                            \
    0),                                         \
        1),                                     \
        270),                                   \
        3700),                                  \
        JSVAL_TO_INT (JSVAL_INT_MAX / 2)),      \
        JSVAL_TO_INT (JSVAL_INT_MAX)),          \
        INT_MAX / 2),                           \
        INT_MAX),                               \
        UINT_MAX / 2),                          \
        UINT_MAX

#define GLONG_VALUES                            \
    LONG_MIN),                                  \
        LONG_MIN / 2),                          \
        LONG_MAX / 2),                          \
        LONG_MAX

#define GULONG_VALUES                           \
    LONG_MAX / 2),                              \
        LONG_MAX),                              \
        ULONG_MAX / 2),                         \
        ULONG_MAX

#define GINT64_VALUES                           \
    G_MININT64),                                \
        G_MININT64 / 2),                        \
        G_MAXINT64 / 2),                        \
        G_MAXINT64

#define GUINT64_VALUES                          \
    G_MAXINT64 / 2),                            \
        G_MAXINT64),                            \
        G_MAXUINT64 / 2),                       \
        G_MAXUINT64

static int jsval_int_values[] = { 
    JSINT_VALUES
};

static double jsval_double_values[] = {
    JSINT_VALUES,
    JSDOUBLE_VALUES
};

static char *jsval_string_values[] = {
    "",
    __FILE__,
    PACKAGE,
    PACKAGE_BUGREPORT,
    PACKAGE_NAME,
    PACKAGE_STRING,
    PACKAGE_TARNAME,
    PACKAGE_VERSION,
    VERSION,
    "I'm not really sure what to test here, really."
};

static int g_value_int_values[] = {
    JSINT_VALUES,
    GINT_VALUES
};

static unsigned int g_value_uint_values[] = {
    GUINT_VALUES
};

static long g_value_long_values[] = {
    JSINT_VALUES,
    GINT_VALUES,
    GLONG_VALUES
};

static unsigned long g_value_ulong_values[] = {
    GUINT_VALUES,
    GULONG_VALUES
};

static gint64 g_value_int64_values[] = {
    JSINT_VALUES,
    GINT_VALUES,
    GLONG_VALUES,
    GINT64_VALUES
};

static guint64 g_value_uint64_values[] = {
    GUINT_VALUES,
    GULONG_VALUES,
    GUINT64_VALUES
};

#define g_value_double_values jsval_double_values
#define g_value_float_values  jsval_double_values

#define g_value_string_values jsval_string_values

static void
setup (void)
{
    GValue dummy = { 0 };
    gval = dummy;
    jval = JSVAL_VOID;
    err = NULL;
}

static void
teardown (void)
{
    g_value_unset (&gval);
    if (err) {
        g_printerr ("Err: %s\n", err->message);
        g_error_free (err);
    }
    fail_unless (err == NULL);
}


START_TEST (test_gom_value_jsval_double)
{
    double val;
    fail_unless (_i < G_N_ELEMENTS (jsval_double_values));
    val = jsval_double_values[_i];
    fail_unless (JS_NewDoubleValue (cx, val, &jval));
    fail_unless (gom_g_value (cx, &gval, jval, &err));
    fail_unless (err == NULL);
    fail_unless (G_VALUE_HOLDS_DOUBLE (&gval));
    fail_unless (val == g_value_get_double (&gval));
}
END_TEST

START_TEST (test_gom_value_jsval_double_nan)
{
    jval = JS_GetNaNValue (cx);
    fail_unless (gom_g_value (cx, &gval, jval, &err));
    fail_unless (err == NULL);
    fail_unless (G_VALUE_HOLDS_DOUBLE (&gval));
    fail_unless (isnan (g_value_get_double (&gval)));
}
END_TEST

START_TEST (test_gom_value_jsval_double_neginf)
{
    jval = JS_GetNegativeInfinityValue (cx);
    fail_unless (gom_g_value (cx, &gval, jval, &err));
    fail_unless (err == NULL);
    fail_unless (G_VALUE_HOLDS_DOUBLE (&gval));
    fail_unless (isinf (g_value_get_double (&gval)));
}
END_TEST

START_TEST (test_gom_value_jsval_double_posinf)
{
    jval = JS_GetPositiveInfinityValue (cx);
    fail_unless (gom_g_value (cx, &gval, jval, &err));
    fail_unless (err == NULL);
    fail_unless (G_VALUE_HOLDS_DOUBLE (&gval));
    fail_unless (isinf (g_value_get_double (&gval)));
}
END_TEST

START_TEST (test_gom_value_jsval_string)
{
    JSString *jstr;
    char *val;
    fail_unless (_i < G_N_ELEMENTS (jsval_string_values));
    val = jsval_string_values[_i];
    fail_if (NULL == (jstr = JS_NewStringCopyZ (cx, val)));
    jval = STRING_TO_JSVAL (jstr);
    fail_unless (JSVAL_IS_STRING (jval));
    fail_unless (gom_g_value (cx, &gval, jval, &err));
    fail_unless (err == NULL);
    fail_unless (G_VALUE_HOLDS_STRING (&gval));
    fail_if (strcmp (val, g_value_get_string (&gval)));
}
END_TEST

START_TEST (test_gom_value_jsval_string_this_file)
{
    JSString *jstr;
    char *val;
    gsize len;

    fail_unless (g_file_get_contents (__FILE__, &val, &len, &err));
    fail_unless (err == NULL);
    fail_if (len == 0);
    fail_if (NULL == (jstr = JS_NewStringCopyZ (cx, val)));
    jval = STRING_TO_JSVAL (jstr);
    fail_unless (JSVAL_IS_STRING (jval));
    fail_unless (gom_g_value (cx, &gval, jval, &err));
    fail_unless (err == NULL);
    fail_unless (G_VALUE_HOLDS_STRING (&gval));
    fail_if (strcmp (val, g_value_get_string (&gval)));
    g_free (val);
}
END_TEST
    

START_TEST (test_gom_value_jsval_boolean_true)
{
    jval = JSVAL_TRUE;
    fail_unless (gom_g_value (cx, &gval, jval, &err));
    fail_unless (err == NULL);
    fail_unless (G_VALUE_HOLDS_BOOLEAN (&gval));
    fail_unless (g_value_get_boolean (&gval));
}
END_TEST

START_TEST (test_gom_value_jsval_boolean_false)
{
    jval = JSVAL_FALSE;
    fail_unless (gom_g_value (cx, &gval, jval, &err));
    fail_unless (err == NULL);
    fail_unless (G_VALUE_HOLDS_BOOLEAN (&gval));
    fail_if (g_value_get_boolean (&gval));
}
END_TEST

START_TEST (test_gom_value_jsval_int)
{
    int val;
    fail_unless (_i < G_N_ELEMENTS (jsval_int_values));
    val = jsval_int_values[_i];
    fail_unless (INT_FITS_IN_JSVAL (val));
    jval = INT_TO_JSVAL (val);
    fail_unless (gom_g_value (cx, &gval, jval, &err));
    fail_unless (err == NULL);
    fail_unless (G_VALUE_HOLDS_INT (&gval));
    fail_unless (val == g_value_get_int (&gval));
}
END_TEST

START_TEST (test_gom_value_g_value_char)
{
    JSString *jstr;
    char *str;
    fail_unless (&gval == g_value_init (&gval, G_TYPE_CHAR));
    g_value_set_char (&gval, _i);
    fail_unless (_i == g_value_get_char (&gval));
    fail_unless (gom_jsval (cx, &jval, &gval, &err));
    fail_unless (err == NULL);
    fail_unless (JSVAL_IS_STRING (jval));
    fail_unless (NULL != (jstr = JSVAL_TO_STRING (jval)));
    fail_unless (NULL != (str = JS_GetStringBytes (jstr)));
    fail_unless (str[0] == _i);
    fail_unless (str[1] == '\0');
}
END_TEST

START_TEST (test_gom_value_g_value_uchar)
{
    JSString *jstr;
    jschar *str;
    fail_unless (&gval == g_value_init (&gval, G_TYPE_UCHAR));
    g_value_set_uchar (&gval, _i);
    fail_unless (_i == g_value_get_uchar (&gval));
    fail_unless (gom_jsval (cx, &jval, &gval, &err));
    fail_unless (err == NULL);
    fail_unless (JSVAL_IS_STRING (jval));
    fail_unless (NULL != (jstr = JSVAL_TO_STRING (jval)));
    fail_unless (NULL != (str = JS_GetStringChars (jstr)));
    fail_unless (str[0] == _i);
    fail_unless (str[1] == '\0');
}
END_TEST

START_TEST (test_gom_value_g_value_boolean_true)
{
    fail_unless (&gval == g_value_init (&gval, G_TYPE_BOOLEAN));
    g_value_set_boolean (&gval, TRUE);
    fail_unless (gom_jsval (cx, &jval, &gval, &err));
    fail_unless (err == NULL);
    fail_unless (JSVAL_IS_BOOLEAN (jval));
    fail_unless (JSVAL_TO_BOOLEAN (jval));
}
END_TEST

START_TEST (test_gom_value_g_value_boolean_false)
{
    fail_unless (&gval == g_value_init (&gval, G_TYPE_BOOLEAN));
    g_value_set_boolean (&gval, FALSE);
    fail_unless (gom_jsval (cx, &jval, &gval, &err));
    fail_unless (err == NULL);
    fail_unless (JSVAL_IS_BOOLEAN (jval));
    fail_if (JSVAL_TO_BOOLEAN (jval));
}
END_TEST

#define CHECK_G_VALUE_NUMBER(typename,type,gtype)                       \
    START_TEST (test_gom_value_g_value_##typename)                      \
    {                                                                   \
        type val;                                                       \
        fail_unless (_i < G_N_ELEMENTS (g_value_##typename##_values));  \
        val = g_value_##typename##_values[_i];                          \
        fail_unless (&gval == g_value_init (&gval, gtype));             \
        g_value_set_##typename (&gval, val);                            \
        fail_unless (gom_jsval (cx, &jval, &gval, &err));               \
        fail_unless (err == NULL);                                      \
        fail_unless (JSVAL_IS_INT (jval) || JSVAL_IS_DOUBLE (jval));    \
        if (JSVAL_IS_INT (jval)) {                                      \
            fail_unless (val == JSVAL_TO_INT (jval));                   \
        } else {                                                        \
            fail_unless (val == *JSVAL_TO_DOUBLE (jval));               \
        }                                                               \
    }                                                                   \
    END_TEST

CHECK_G_VALUE_NUMBER (int),   int),    G_TYPE_INT);
CHECK_G_VALUE_NUMBER (uint),  guint),  G_TYPE_UINT);
CHECK_G_VALUE_NUMBER (long),  long),   G_TYPE_LONG);
CHECK_G_VALUE_NUMBER (ulong), gulong), G_TYPE_ULONG);
CHECK_G_VALUE_NUMBER (int64), gint64), G_TYPE_INT64);
CHECK_G_VALUE_NUMBER (uint64, guint64, G_TYPE_UINT64);
CHECK_G_VALUE_NUMBER (float), float),  G_TYPE_FLOAT);
CHECK_G_VALUE_NUMBER (double, double), G_TYPE_DOUBLE);

START_TEST (test_gom_value_g_value_string)
{
    char *val;
    fail_unless (_i < G_N_ELEMENTS (g_value_string_values));
    val = g_value_string_values[_i];
    fail_unless (&gval == g_value_init (&gval, G_TYPE_STRING));
    g_value_set_static_string (&gval, val);
    fail_unless (gom_jsval (cx, &jval, &gval, &err));
    fail_unless (err == NULL);
    fail_unless (JSVAL_IS_STRING (jval));
    fail_if (strcmp (val, JS_GetStringBytes (JSVAL_TO_STRING (jval))));
}
END_TEST

START_TEST (test_gom_value_g_value_string_this_file)
{
    char *val;
    gsize len;

    fail_unless (g_file_get_contents (__FILE__, &val, &len, &err));
    fail_unless (err == NULL);
    fail_if (len == 0);
    fail_unless (&gval == g_value_init (&gval, G_TYPE_STRING));
    g_value_set_static_string (&gval, val);
    fail_unless (gom_jsval (cx, &jval, &gval, &err));
    fail_unless (err == NULL);
    fail_unless (JSVAL_IS_STRING (jval));
    fail_if (strcmp (val, JS_GetStringBytes (JSVAL_TO_STRING (jval))));
    g_free (val);
}
END_TEST

#define _LOOPED_CASE(typename, type, valname)                           \
    tcase = tcase_create (#type);                                       \
    tcase_add_checked_fixture (tcase, setup, teardown);                 \
    tcase_add_loop_test (tcase, test_gom_value_##valname##_##typename), \
                         0, G_N_ELEMENTS (valname##_##typename##_values)); \
    suite_add_tcase (s, tcase);

#define JS_LOOPED_CASE(typename, type) _LOOPED_CASE(typename, type, jsval)
#define G_LOOPED_CASE(typename, type) _LOOPED_CASE(typename, type, g_value)

Suite *
gom_value_suite (void)
{
    Suite *s;
    TCase *tcase;

    s = suite_create ("GomValue");

    JS_LOOPED_CASE (double, JSVAL_DOUBLE);
    tcase_add_test (tcase, test_gom_value_jsval_double_nan);
    tcase_add_test (tcase, test_gom_value_jsval_double_neginf);
    tcase_add_test (tcase, test_gom_value_jsval_double_posinf);

    JS_LOOPED_CASE (string, JSVAL_STRING);
    tcase_add_test (tcase, test_gom_value_jsval_string_this_file);

    tcase = tcase_create ("JSVAL_BOOLEAN");
    tcase_add_checked_fixture (tcase, setup, teardown);
    tcase_add_test (tcase, test_gom_value_jsval_boolean_true);
    tcase_add_test (tcase, test_gom_value_jsval_boolean_false);
    suite_add_tcase (s, tcase);

    JS_LOOPED_CASE (int, JSVAL_INT);

    tcase = tcase_create ("G_TYPE_CHAR");
    tcase_add_checked_fixture (tcase, setup, teardown);
    tcase_add_loop_test (tcase, test_gom_value_g_value_char, CHAR_MIN, CHAR_MAX);
    suite_add_tcase (s, tcase);

    tcase = tcase_create ("G_TYPE_UCHAR");
    tcase_add_checked_fixture (tcase, setup, teardown);
    tcase_add_loop_test (tcase, test_gom_value_g_value_uchar, 0, UCHAR_MAX);
    suite_add_tcase (s, tcase);

    tcase = tcase_create ("G_TYPE_BOOLEAN");
    tcase_add_checked_fixture (tcase, setup, teardown);
    tcase_add_test (tcase, test_gom_value_g_value_boolean_true);
    tcase_add_test (tcase, test_gom_value_g_value_boolean_false);
    suite_add_tcase (s, tcase);
    
    G_LOOPED_CASE (int),   G_TYPE_INT);
    G_LOOPED_CASE (uint),  G_TYPE_UINT);
    G_LOOPED_CASE (long),  G_TYPE_LONG);
    G_LOOPED_CASE (ulong), G_TYPE_ULONG);
    G_LOOPED_CASE (int64), G_TYPE_INT64);
    G_LOOPED_CASE (uint64, G_TYPE_UINT64);
    G_LOOPED_CASE (float), G_TYPE_FLOAT);
    G_LOOPED_CASE (double, G_TYPE_DOUBLE);

    G_LOOPED_CASE (string, G_TYPE_STRING);
    tcase_add_test (tcase, test_gom_value_g_value_string_this_file);

    return s;
}
