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

#include "gom/gomjsmouseevent.h"

#include "gom/dom/gomdomexception.h"
#include "gom/dom/gommouseevent.h"
#include "gom/gomjsexception.h"
#include "gom/gomjsobject.h"
#include "gom/gomjsuievent.h"

#include "gommacros.h"

#include <glib.h>

static void
gom_js_mouse_event_finalize (JSContext *cx, JSObject *obj)
{
    GomJSUIEventClass.finalize (cx, obj);
}

JSClass GomJSMouseEventClass = {
    "MouseEvent", JSCLASS_NEW_ENUMERATE,

    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    (JSEnumerateOp)gom_js_object_enumerate,
    JS_ResolveStub,
    JS_ConvertStub,
    gom_js_mouse_event_finalize
};

static JSPropertySpec gom_js_mouse_event_props[] = { { NULL } };

static JSBool
gom_js_event_init_mouse_event (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_event_get_modifier_state (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_event_init_mouse_event_ns (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSFunctionSpec gom_js_mouse_event_funcs[] = {
    { "initMouseEvent",   gom_js_event_init_mouse_event,    15 },
    { "getModifierState", gom_js_event_get_modifier_state,  1  },
    { "initMouseEventNS", gom_js_event_init_mouse_event_ns, 16 },
    { NULL }
};

static JSBool
gom_js_mouse_event_construct (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_TRUE;
}

JSObject *
gom_js_mouse_event_init_class (JSContext *cx, JSObject *obj)
{
    gom_js_object_register_js_class (cx, GOM_TYPE_MOUSE_EVENT, &GomJSMouseEventClass);
    return JS_InitClass (cx, obj,
                         JS_ConstructObject (cx, &GomJSUIEventClass, NULL, NULL),
                         &GomJSMouseEventClass, gom_js_mouse_event_construct, 0,
                         gom_js_mouse_event_props, gom_js_mouse_event_funcs, NULL, NULL);
}
