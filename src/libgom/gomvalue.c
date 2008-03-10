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

#include <gom/gomvalue.h>

#include <gom/gomjsobject.h>

GQuark
gom_value_error_quark (void)
{
    return g_quark_from_static_string ("gom-value-error-quark");
}

gboolean
gom_g_value (JSContext *cx, GValue *gval, jsval jval, GError **error)
{
    switch (JSVAL_TAG (jval)) {
    case JSVAL_OBJECT: {
        GObject *gobj;
        gobj = gom_js_object_get_g_object (cx, JSVAL_TO_OBJECT (jval));
        if (!gobj) {
            g_set_error (error, GOM_VALUE_ERROR, GOM_VALUE_ERROR_G_OBJECT_NOT_FOUND,
                         "Cannot get GObject from JSObject %p (%s)",
                         JSVAL_TO_OBJECT (jval),
                         JS_GET_CLASS (cx, JSVAL_TO_OBJECT (jval))->name);
            return FALSE;
        }
        break;
    }
    case JSVAL_DOUBLE:
        g_value_init (gval, G_TYPE_DOUBLE);
        g_value_set_double (gval, *JSVAL_TO_DOUBLE (jval));
        break;
    case JSVAL_STRING:
        g_value_init (gval, G_TYPE_STRING);
        g_value_set_string (gval, JS_GetStringBytes (JSVAL_TO_STRING (jval)));
        break;
    case JSVAL_BOOLEAN:
        g_value_init (gval, G_TYPE_BOOLEAN);
        g_value_set_boolean (gval, JSVAL_TO_BOOLEAN (jval));
        break;
    default:
        /* since these are 31-bit, they can have multiple tag values;
         * only the lsb is significant in determining if it's a
         * JSVAL_INT */
        if (JSVAL_IS_INT (jval)) {
            g_value_init (gval, G_TYPE_INT);
            g_value_set_int (gval, JSVAL_TO_INT (jval));
            break;
        }

        g_set_error (error, GOM_VALUE_ERROR, GOM_VALUE_ERROR_UNKNOWN_JSVAL_TAG,
                     "Unknown jsval tag: %#x in jsval %p",
                     (int)JSVAL_TAG (jval), GINT_TO_POINTER (jval));
        return FALSE;
    }

    return TRUE;
}

gboolean
gom_jsval (JSContext *cx, jsval *jval, const GValue *gval, GError **error)
{
    gboolean ret = FALSE;
    JSString *jstr;
    jschar jc;
    char c;

    g_return_val_if_fail (jval != NULL, FALSE);
    g_return_val_if_fail (gval != NULL, FALSE);

    switch (G_VALUE_TYPE (gval)) {
    case G_TYPE_CHAR:
        c = g_value_get_char (gval);
        jstr = JS_NewStringCopyN (cx, &c, 1);
        ret = jstr != NULL;
        *jval = STRING_TO_JSVAL (jstr);
        break;
    case G_TYPE_UCHAR:
        jc = g_value_get_uchar (gval);
        jstr = JS_NewUCStringCopyN (cx, &jc, 1);
        ret = jstr != NULL;
        *jval = STRING_TO_JSVAL (jstr);
        break;
    case G_TYPE_BOOLEAN:
        *jval = BOOLEAN_TO_JSVAL (g_value_get_boolean (gval));
        ret = TRUE;
        break;

    case G_TYPE_INT:
        ret = JS_NewNumberValue (cx, g_value_get_int (gval), jval);
        break;
    case G_TYPE_UINT:
        ret = JS_NewNumberValue (cx, g_value_get_uint (gval), jval);
        break;

    case G_TYPE_LONG:
        ret = JS_NewNumberValue (cx, g_value_get_long (gval), jval);
        break;
    case G_TYPE_ULONG:
        ret = JS_NewNumberValue (cx, g_value_get_ulong (gval), jval);
        break;

    case G_TYPE_INT64:
        ret = JS_NewNumberValue (cx, g_value_get_int64 (gval), jval);
        break;
    case G_TYPE_UINT64:
        ret = JS_NewNumberValue (cx, g_value_get_uint64 (gval), jval);
        break;

    case G_TYPE_FLOAT:
        ret = JS_NewNumberValue (cx, g_value_get_float (gval), jval);
        break;
    case G_TYPE_DOUBLE:
        ret = JS_NewNumberValue (cx, g_value_get_double (gval), jval);
        break;

    case G_TYPE_STRING:
        *jval = STRING_TO_JSVAL (JS_NewStringCopyZ (cx, g_value_get_string (gval)));
        ret = TRUE;
        break;

    default:
        if (G_VALUE_HOLDS (gval, G_TYPE_OBJECT)) {
            GObject  *gobj;
            JSObject *jsobj;
            gobj = g_value_get_object (gval);
            if (!gobj) {
                *jval = JSVAL_NULL;
            } else {
                jsobj = gom_js_object_get_or_create_js_object (cx, gobj);
                if (!jsobj) {
                    g_set_error (error, GOM_VALUE_ERROR, GOM_VALUE_ERROR_JS_OBJECT_NOT_FOUND,
                                 "Cannot get JSObject from Object %p (%s)",
                                 gobj, g_type_name (G_TYPE_FROM_INSTANCE (gobj)));
                    return FALSE;
                }
                *jval = OBJECT_TO_JSVAL (jsobj);
            }
            ret = TRUE;
        }
    }

    if (!ret) {
        g_set_error (error, GOM_VALUE_ERROR, GOM_VALUE_ERROR_UNHANDLED_G_TYPE,
                     "Cannot convert from GType %s to jsval",
                     G_VALUE_TYPE_NAME (gval));
    }
        
    return ret;
}
