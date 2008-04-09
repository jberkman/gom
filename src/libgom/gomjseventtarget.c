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

#include "gom/gomjseventtarget.h"

#include "gom/dom/gomdomexception.h"
#include "gom/dom/gomeventtarget.h"
#include "gom/gomjseventlistener.h"
#include "gom/gomjsexception.h"
#include "gom/gomjsobject.h"

#include "gommacros.h"

#include <glib.h>

JSClass GomJSEventTargetClass = {
    "EventTarget", JSCLASS_NEW_ENUMERATE,

    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    (JSEnumerateOp)gom_js_object_enumerate,
    JS_ResolveStub,
    JS_ConvertStub,
    JS_FinalizeStub
};

static JSPropertySpec gom_js_event_target_props[] = { { NULL } };

static JSBool
gom_js_event_target_add_event_listener (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomEventTarget   *target;
    JSObject         *js_listener;
    char             *type_name;
    GomEventListener *listener;
    JSBool            use_capture;  

    target = gom_js_object_get_g_object (cx, obj);
    if (!GOM_IS_EVENT_TARGET (target)) {
        return JS_FALSE;
    }
    if (!JS_ConvertArguments (cx, argc, argv, "sob", &type_name, &js_listener, &use_capture)) {
        return JS_FALSE;
    }

    listener = gom_js_object_get_g_object (cx, js_listener);
    if (!listener) {
        listener = g_object_new (GOM_TYPE_JS_EVENT_LISTENER,
                                 "js-context", cx,
                                 "js-object",  js_listener,
                                 NULL);
    } else if (!GOM_IS_EVENT_LISTENER (listener)) {
        return JS_FALSE;
    }

    gom_event_target_add_event_listener (target, type_name, listener, use_capture);

    return JS_TRUE;
}

static JSBool
gom_js_event_target_remove_event_listener (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_event_target_dispatch_event (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomEventTarget *target;
    GomEvent *event;
    JSObject *jsevent;
    gboolean ret;
    GError *error = NULL;

    target = gom_js_object_get_g_object (cx, obj);
    if (!GOM_IS_EVENT_TARGET (target)) {
        return JS_FALSE;
    }

    if (!JS_ConvertArguments (cx, argc, argv, "o", &jsevent)) {
        return JS_FALSE;
    }

    event = gom_js_object_get_g_object (cx, jsevent);
    if (!GOM_IS_EVENT (event)) {
        return JS_FALSE;
    }

    ret = gom_event_target_dispatch_event (target, event, &error);
    if (error) {
        return gom_js_exception_set_error (cx, &error);
    }

    *rval = INT_TO_JSVAL (ret);
    return JS_TRUE;
}

static JSBool
gom_js_event_target_add_event_listener_ns (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_event_target_remove_event_listener_ns (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_event_target_will_trigger_ns (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_event_target_has_event_listener_ns (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSFunctionSpec gom_js_event_target_funcs[] = {
    { "addEventListener",      gom_js_event_target_add_event_listener,       3 },
    { "removeEventListener",   gom_js_event_target_remove_event_listener,    3 },
    { "dispatchEvent",         gom_js_event_target_dispatch_event,           1 },
    { "addEventListenerNS",    gom_js_event_target_add_event_listener_ns,    5 },
    { "removeEventListenerNS", gom_js_event_target_remove_event_listener_ns, 4 },
    { "willTriggerNS",         gom_js_event_target_will_trigger_ns,          2 },
    { "hasEventListenerNS",    gom_js_event_target_has_event_listener_ns,    2 },
    { NULL }
};

static JSBool
gom_js_event_target_construct (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_TRUE;
}

JSObject *
gom_js_event_target_init_class (JSContext *cx, JSObject *obj)
{
    gom_js_object_register_js_class (cx, GOM_TYPE_EVENT_TARGET, &GomJSEventTargetClass);
    return JS_InitClass (cx, obj,
                         JS_ConstructObject (cx, &GomJSObjectClass, NULL, NULL),
                         &GomJSEventTargetClass, gom_js_event_target_construct, 0,
                         gom_js_event_target_props, gom_js_event_target_funcs, NULL, NULL);
}
