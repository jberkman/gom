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

#include "gom/gomjsgerrorexception.h"

#include "gommacros.h"

#include <glib.h>

static void
gom_js_g_error_exception_finalize (JSContext *cx, JSObject *obj)
{
    GError *err;
    g_print ("%s:%d:%s(%s %p)\n",
             __FILE__, __LINE__, __FUNCTION__,
             JS_GET_CLASS (cx, obj)->name, obj);
    err = JS_GetPrivate (cx, obj);
    if (err) {
        g_error_free (err);
    }
}

JSClass GomJSGErrorExceptionClass = {
    "GErrorException", JSCLASS_HAS_PRIVATE,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_EnumerateStub,
    JS_ResolveStub,
    JS_ConvertStub,
    gom_js_g_error_exception_finalize
};

static JSBool
gom_js_g_error_exception_get_code (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    GError *err = JS_GetPrivate (cx, obj);
    *vp = INT_TO_JSVAL (err ? err->code : 0);
    return JS_TRUE;
}    

static JSPropertySpec gom_js_g_error_exception_props[] = { 
    { "code", 0, JSPROP_PERMANENT | JSPROP_READONLY, gom_js_g_error_exception_get_code },
    { NULL }
};

static JSBool
gom_js_g_error_exception_to_string (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GError *err = JS_GetPrivate (cx, obj);
    *rval = STRING_TO_JSVAL (JS_NewStringCopyZ (cx, err ? err->message : "No error"));
    return JS_TRUE;
}

static JSFunctionSpec gom_js_g_error_exception_funcs[] = {
    { "toString", gom_js_g_error_exception_to_string, 0 },
    { NULL }
};

static JSBool
gom_js_g_error_exception_construct (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_TRUE;
}

JSObject *
gom_js_g_error_exception_init_class (JSContext *cx, JSObject *obj)
{
    return JS_InitClass (cx, obj,
                         JS_ConstructObject (cx, NULL, NULL, NULL),
                         &GomJSGErrorExceptionClass, gom_js_g_error_exception_construct, 0,
                         gom_js_g_error_exception_props, gom_js_g_error_exception_funcs,
                         NULL, NULL);
}
