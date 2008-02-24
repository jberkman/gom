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

#include <string.h>

#include <gom/gomvalue.h>
#include <jsapi.h>

#include <stdlib.h>
#include <math.h>

static GError *err;
static GValue gval = { 0 };
static jsval  jval;

static int jsval_int_values[] = { 
    JSVAL_TO_INT (JSVAL_INT_MIN), 
    JSVAL_TO_INT (JSVAL_INT_MIN / 2),
    -3700,
    -270,
    -1,
    0,
    1,
    270,
    3700,
    JSVAL_TO_INT (JSVAL_INT_MAX / 2),
    JSVAL_TO_INT (JSVAL_INT_MAX)
};

#define N_JSVAL_INT_VALUES (sizeof (jsval_int_values) / sizeof (int))

static double jsval_double_values[] = {
    G_E, -G_E,
    G_LN2, -G_LN2,
    G_LN10, -G_LN10,
    G_PI, -G_PI,
    G_PI_2, -G_PI_2,
    G_PI_4, -G_PI_4,
    G_SQRT2, -G_SQRT2
};

#define N_JSVAL_DOUBLE_VALUES (sizeof (jsval_double_values) / sizeof (double))

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

#define N_JSVAL_STRING_VALUES (sizeof (jsval_string_values) / sizeof (double))

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
    fail_unless (_i < N_JSVAL_DOUBLE_VALUES);
    val = jsval_double_values[_i];
    fail_unless (JS_NewDoubleValue (cx, val, &jval));
    fail_unless (gom_g_value (cx, &gval, jval, &err));
    fail_unless (err == NULL);
    fail_unless (G_VALUE_HOLDS_DOUBLE (&gval));
    fail_unless (val == g_value_get_double (&gval));
}
END_TEST

START_TEST (test_gom_value_jsval_double_ints)
{
    double val;
    fail_unless (_i < N_JSVAL_INT_VALUES);
    val = jsval_int_values[_i];
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
    fail_unless (_i < N_JSVAL_STRING_VALUES);
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
    fail_unless (_i < N_JSVAL_INT_VALUES);
    val = jsval_int_values[_i];
    fail_unless (INT_FITS_IN_JSVAL (val));
    jval = INT_TO_JSVAL (val);
    fail_unless (gom_g_value (cx, &gval, jval, &err));
    fail_unless (err == NULL);
    fail_unless (G_VALUE_HOLDS_INT (&gval));
    fail_unless (val == g_value_get_int (&gval));
}
END_TEST

Suite *
gom_value_suite (void)
{
    Suite *s;
    TCase *tcase;

    s = suite_create ("GomValue");

    tcase = tcase_create ("JSVAL_DOUBLE");
    tcase_add_checked_fixture (tcase, setup, teardown);
    tcase_add_loop_test (tcase, test_gom_value_jsval_double_ints, 0, N_JSVAL_INT_VALUES);
    tcase_add_loop_test (tcase, test_gom_value_jsval_double, 0, N_JSVAL_DOUBLE_VALUES);
    tcase_add_test (tcase, test_gom_value_jsval_double_nan);
    tcase_add_test (tcase, test_gom_value_jsval_double_neginf);
    tcase_add_test (tcase, test_gom_value_jsval_double_posinf);
    suite_add_tcase (s, tcase);

    tcase = tcase_create ("JSVAL_STRING");
    tcase_add_checked_fixture (tcase, setup, teardown);
    tcase_add_loop_test (tcase, test_gom_value_jsval_string, 0, N_JSVAL_STRING_VALUES);
    tcase_add_test (tcase, test_gom_value_jsval_string_this_file);
    suite_add_tcase (s, tcase);

    tcase = tcase_create ("JSVAL_BOOLEAN");
    tcase_add_checked_fixture (tcase, setup, teardown);
    tcase_add_test (tcase, test_gom_value_jsval_boolean_true);
    tcase_add_test (tcase, test_gom_value_jsval_boolean_false);
    suite_add_tcase (s, tcase);

    tcase = tcase_create ("JSVAL_INT");
    tcase_add_checked_fixture (tcase, setup, teardown);
    tcase_add_loop_test (tcase, test_gom_value_jsval_int, 0, N_JSVAL_INT_VALUES);
    suite_add_tcase (s, tcase);

    return s;
}
