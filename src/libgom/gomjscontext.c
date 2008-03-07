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

#include <gom/gomjscontext.h>

#include <gom/gomjsdocument.h>
#include <gom/gomjselement.h>
#include <gom/gomjsnode.h>
#include <gom/gomjsobject.h>
#include <gom/gomjswindow.h>

#include <glib-object.h>

static void
gom_js_context_init_standard_classes (JSContext *cx, JSObject *obj)
{
    gom_js_object_init_class (cx, obj);
    gom_js_node_init_class (cx, obj);
    gom_js_element_init_class (cx, obj);
    gom_js_document_init_class (cx, obj);
}

static gboolean
gom_js_context_init_private (JSContext *cx)
{
    if (JS_GetContextPrivate (cx)) {
        JS_SetPendingException (cx, STRING_TO_JSVAL (JS_NewStringCopyZ (cx, "Context already has private data")));
        return FALSE;
    }
    JS_SetContextPrivate (cx, g_object_new (G_TYPE_OBJECT, NULL));
    return TRUE;
}

JSObject *
gom_js_context_init (JSContext *cx)
{
    JSObject *window;

    if (!gom_js_context_init_private (cx)) {
        return NULL;
    }

    window = JS_NewObject (cx, &GomJSWindowClass, NULL, NULL);
    if (!window) {
        g_object_unref (GOM_JS_CONTEXT_PRIV (cx));
        JS_SetContextPrivate (cx, NULL);
        return NULL;
    }

    JS_InitStandardClasses (cx, window);

    gom_js_window_init_object (cx, window);
    gom_js_context_init_standard_classes (cx, window);

    return window;
}
