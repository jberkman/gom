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

#include "gom/gomjskeyboardevent.h"

#include "gom/dom/gomdomexception.h"
#include "gom/dom/gomkeyboardevent.h"
#include "gom/gomjsexception.h"
#include "gom/gomjsobject.h"
#include "gom/gomjsuievent.h"

#include "gommacros.h"

#include <glib.h>

JSClass GomJSKeyboardEventClass = {
    "KeyboardEvent", JSCLASS_NEW_ENUMERATE,

    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    (JSEnumerateOp)gom_js_object_enumerate,
    JS_ResolveStub,
    JS_ConvertStub,
    JS_FinalizeStub
};

static JSPropertySpec gom_js_keyboard_event_props[] = { { NULL } };

static JSBool
gom_js_event_init_keyboard_event (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomKeyboardEvent *evt;

    char *type, *identifier, *modifiers;
    int32 location;
    JSBool bubbles, cancelable;
    GomAbstractView *gview;
    JSObject *view;

    evt = gom_js_object_get_g_object (cx, obj);
    if (!GOM_IS_KEYBOARD_EVENT (evt)) {
        return JS_FALSE;
    }
    
    if (!JS_ConvertArguments (cx, argc, argv, "sbbosis",
                              &type, &bubbles, &cancelable, &view,
                              &identifier, &location, &modifiers)) {
        return JS_FALSE;
    }

    if (view == JSVAL_NULL) {
        gview = NULL;
    } else {
        gview = gom_js_object_get_g_object (cx, view);
        if (!GOM_IS_ABSTRACT_VIEW (gview)) {
            return JS_FALSE;
        }
    }

    gom_keyboard_event_init_keyboard_event (evt, type, bubbles, cancelable, gview, identifier, location, modifiers);
    return JS_TRUE;
}

static JSBool
gom_js_event_get_modifier_state (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_event_init_keyboard_event_ns (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSFunctionSpec gom_js_keyboard_event_funcs[] = {
    { "initKeyboardEvent",   gom_js_event_init_keyboard_event,    7 },
    { "getModifierState",    gom_js_event_get_modifier_state,     1 },
    { "initKeyboardEventNS", gom_js_event_init_keyboard_event_ns, 8 },
    { NULL }
};

static JSBool
gom_js_keyboard_event_construct (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_TRUE;
}

JSObject *
gom_js_keyboard_event_init_class (JSContext *cx, JSObject *obj)
{
    gom_js_object_register_js_class (cx, GOM_TYPE_KEYBOARD_EVENT, &GomJSKeyboardEventClass);
    return JS_InitClass (cx, obj,
                         JS_ConstructObject (cx, &GomJSUIEventClass, NULL, NULL),
                         &GomJSKeyboardEventClass, gom_js_keyboard_event_construct, 0,
                         gom_js_keyboard_event_props, gom_js_keyboard_event_funcs, NULL, NULL);
}
