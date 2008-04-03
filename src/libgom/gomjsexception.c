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

#include "gom/gomjsexception.h"

#include "gom/dom/gomdomexception.h"
#include "gom/dom/gomeventexception.h"
#include "gom/gomjsdomexception.h"
#include "gom/gomjseventexception.h"
#include "gom/gomjsgerrorexception.h"

#include "gommacros.h"

#include <glib.h>

GOM_DEFINE_QUARK(js_exception)

gboolean
gom_js_exception_get_error (JSContext *cx, GError **error)
{
    JSString *str;
    GError *err = NULL;
    jsval exc;

    if (!JS_GetPendingException (cx, &exc)) {
        return FALSE;
    }
    if (JSVAL_IS_OBJECT (exc)) {
        if (JS_GET_CLASS (cx, JSVAL_TO_OBJECT (exc)) == &GomJSDOMExceptionClass ||
            JS_GET_CLASS (cx, JSVAL_TO_OBJECT (exc)) == &GomJSEventExceptionClass ||
            JS_GET_CLASS (cx, JSVAL_TO_OBJECT (exc)) == &GomJSGErrorExceptionClass) {
            err = JS_GetPrivate (cx, JSVAL_TO_OBJECT (exc));
        }
    }
    if (err) {
        if (*error) {
            *error = g_error_copy (err);
        }
    } else {
        str = JS_ValueToString (cx, exc);
        if (!str) {
            return FALSE;
        }
        g_set_error (error, GOM_JS_EXCEPTION_ERROR, GOM_JS_EXCEPTION_ERROR_GENERIC,
                     "%s", JS_GetStringBytes (str));
    }
    return TRUE;
}

void
gom_js_exception_set_error (JSContext *cx, GError *error)
{
    JSObject *obj;
    JSClass  *klass;
    klass = (error->domain == GOM_DOM_EXCEPTION_ERROR)
        ? &GomJSDOMExceptionClass
        : (error->domain == GOM_EVENT_EXCEPTION_ERROR)
        ? &GomJSEventExceptionClass
        : &GomJSGErrorExceptionClass;
    obj = klass ? JS_ConstructObject (cx, klass, NULL, NULL) : NULL;
    if (obj) {
        JS_SetPrivate (cx, obj, g_error_copy (error));
    }
    JS_SetPendingException (cx, obj 
                            ? OBJECT_TO_JSVAL (obj)
                            : STRING_TO_JSVAL (JS_NewStringCopyZ (cx, error->message)));
}
