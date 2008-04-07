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

#include "gom/gomjsevent.h"

#include "gom/dom/gomdomexception.h"
#include "gom/dom/gomevent.h"
#include "gom/gomjsexception.h"
#include "gom/gomjsobject.h"

#include "gommacros.h"

#include <glib.h>

JSClass GomJSEventClass = {
    "Event", JSCLASS_NEW_ENUMERATE,

    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    (JSEnumerateOp)gom_js_object_enumerate,
    JS_ResolveStub,
    JS_ConvertStub,
    JS_FinalizeStub
};

static JSPropertySpec gom_js_event_props[] = { { NULL } };

static JSBool
gom_js_event_stop_propagation (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomEvent *evt = gom_js_object_get_g_object (cx, obj);
    if (!GOM_IS_EVENT (evt)) {
        return JS_FALSE;
    }
    gom_event_stop_propagation (evt);
    return JS_TRUE;
}

static JSBool
gom_js_event_prevent_default (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomEvent *evt = gom_js_object_get_g_object (cx, obj);
    if (!GOM_IS_EVENT (evt)) {
        return JS_FALSE;
    }
    gom_event_prevent_default (evt);
    return JS_TRUE;
}

static JSBool
gom_js_event_init_event (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_event_is_custom (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_event_stop_immediate_propagation (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_event_is_default_prevented (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_event_init_event_ns (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSFunctionSpec gom_js_event_funcs[] = {
    { "stopPropagation",          gom_js_event_stop_propagation,           0 },
    { "preventDefault",           gom_js_event_prevent_default,            0 },
    { "initEvent",                gom_js_event_init_event,                 3 },
    { "isCustom",                 gom_js_event_is_custom,                  0 },
    { "stopImmediatePropagation", gom_js_event_stop_immediate_propagation, 0 },
    { "isDefaultPrevented",       gom_js_event_is_default_prevented,       0 },
    { "initEventNS",              gom_js_event_init_event_ns,              4 },
    { NULL },
};

static JSBool
gom_js_event_construct (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_TRUE;
}

JSObject *
gom_js_event_init_class (JSContext *cx, JSObject *obj)
{
    gom_js_object_register_js_class (cx, GOM_TYPE_EVENT, &GomJSEventClass);
    return JS_InitClass (cx, obj,
                         JS_ConstructObject (cx, &GomJSObjectClass, NULL, NULL),
                         &GomJSEventClass, gom_js_event_construct, 0,
                         gom_js_event_props, gom_js_event_funcs, NULL, NULL);
}
