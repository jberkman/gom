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

#include <gom/gomjsexception.h>
#include <glib.h>
#include "gommacros.h"

GOM_DEFINE_QUARK(js_exception)

gboolean
gom_js_exception_get_error (JSContext *cx, GError **error)
{
    JSString *str;
    jsval exc;

    if (!JS_GetPendingException (cx, &exc)) {
        return FALSE;
    }

    str = JS_ValueToString (cx, exc);
    if (!str) {
        return FALSE;
    }

    g_set_error (error, GOM_JS_EXCEPTION_ERROR, GOM_JS_EXCEPTION_ERROR_GENERIC,
                 "%s", JS_GetStringBytes (str));

    return TRUE;
}

void
gom_js_exception_set_error (JSContext *cx, GError  *error)
{
    JS_SetPendingException (cx, STRING_TO_JSVAL (JS_NewStringCopyZ (cx, error->message)));
}
