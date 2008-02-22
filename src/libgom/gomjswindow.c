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

#include <gom/gomjswindow.h>

#include <gtk/gtk.h>

static struct JSClass GomJSWindowClass = {
    "Window", 0,

    JS_PropertyStub, JS_PropertyStub,
    JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub,
    JS_ConvertStub, JS_FinalizeStub
};

static JSPropertySpec window_props[] = {
    { NULL }
};

static JSBool
window_alert (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GtkWidget *dialog;
    char *str;

    if (!JS_ConvertArguments (cx, argc, argv, "s", &str)) {
        return JS_FALSE;
    }

    dialog = gtk_message_dialog_new (NULL, 0,
                                     GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
                                     "JavaScript Message:\n\n%s", str);
    gtk_window_set_title (GTK_WINDOW (dialog), "Gom "VERSION);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);

    return JS_TRUE;
}

static JSBool
window_quit (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    gtk_main_quit ();
    return JS_TRUE;
}

static JSFunctionSpec window_funcs[] = {
    { "alert", window_alert, 1 },
    { "quit", window_quit, 0 },
    { NULL }
};
#if 0
JSObject *
gom_js_window_init_class (JSContext *cx, JSObject *obj)
{
    return JS_InitClass (cx, obj, NULL, &GomJSWindowClass, NULL, 0,
                         window_props, window_funcs, NULL, NULL);
}
#endif

JSObject *
gom_js_window_new_global (JSContext *cx)
{
    JSObject *window = JS_NewObject (cx, &GomJSWindowClass, NULL, NULL);
    if (!window) {
        return NULL;
    }

    JS_InitStandardClasses (cx, window);

    JS_DefineProperties (cx, window, window_props);
    JS_DefineFunctions (cx, window, window_funcs);
    
    JS_DefineProperty (cx, window, "window", OBJECT_TO_JSVAL (window), NULL, NULL,
                       JSPROP_READONLY | JSPROP_PERMANENT);

    return window;
}
