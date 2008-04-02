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

#include "gom/gomjsdomexception.h"

#include "gom/gomjsgerrorexception.h"
#include "gom/dom/gomdombuiltins.h"

#include "gommacros.h"

#include <glib-object.h>

JSClass GomJSDOMExceptionClass = {
    "DOMException", JSCLASS_HAS_PRIVATE,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_EnumerateStub,
    JS_ResolveStub,
    JS_ConvertStub,
    JS_FinalizeStub
};

static JSBool
gom_js_dom_exception_get_enum (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    *vp = id;
    return JS_TRUE;
}

static JSPropertySpec gom_js_dom_exception_props[] = { { NULL } };
static JSFunctionSpec gom_js_dom_exception_funcs[] = { { NULL } };

static JSBool
gom_js_dom_exception_construct (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_TRUE;
}

JSObject *
gom_js_dom_exception_init_class (JSContext *cx, JSObject *obj)
{
    GEnumClass *enums;
    JSObject *proto;
    JSObject *ctor;
    guint i;

    proto = JS_InitClass (cx, obj,
                          JS_ConstructObject (cx, &GomJSGErrorExceptionClass, NULL, NULL),
                          &GomJSDOMExceptionClass, gom_js_dom_exception_construct, 0,
                          gom_js_dom_exception_props, gom_js_dom_exception_funcs,
                          NULL, NULL);

    ctor = JS_GetConstructor (cx, proto);

    enums = g_type_class_ref (GOM_TYPE_EXCEPTION_CODE);
    for (i = 0; i < enums->n_values; i++) {
        JS_DefinePropertyWithTinyId (cx, ctor,
                                     &enums->values[i].value_name[4],
                                     enums->values[i].value,
                                     enums->values[i].value,
                                     gom_js_dom_exception_get_enum, NULL,
                                     JSPROP_READONLY | JSPROP_PERMANENT);
    }

    return proto;
}
