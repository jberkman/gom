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

#include "gom/dom/gomdombuiltins.h"
#include "gom/gomjsgerrorexception.h"
#include "gom/gomjsobject.h"

#include "gommacros.h"

#include <glib-object.h>

static void
gom_js_dom_exception_finalize (JSContext *cx, JSObject *obj)
{
    GomJSGErrorExceptionClass.finalize (cx, obj);
}

JSClass GomJSDOMExceptionClass = {
    "DOMException", JSCLASS_HAS_PRIVATE,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_EnumerateStub,
    JS_ResolveStub,
    JS_ConvertStub,
    gom_js_dom_exception_finalize
};

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
    return gom_js_object_define_static_enums (
        cx,
        JS_InitClass (cx, obj,
                      JS_ConstructObject (cx, &GomJSGErrorExceptionClass, NULL, NULL),
                      &GomJSDOMExceptionClass, gom_js_dom_exception_construct, 0,
                      gom_js_dom_exception_props, gom_js_dom_exception_funcs,
                      NULL, NULL),
        GOM_TYPE_EXCEPTION_CODE);
}
