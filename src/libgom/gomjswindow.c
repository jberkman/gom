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

#include <gom/gomjscontext.h>
#include <gommacros.h>

#include <gtk/gtk.h>

#include <string.h>

GQuark gom_js_window_sources_quark (void);

GOM_DEFINE_QUARK (js_window_sources);
#define SOURCES_QUARK (gom_js_window_sources_quark ())
#define SOURCES(cx) ((GPtrArray *)GOM_JS_CONTEXT_GET_QDATA (cx, SOURCES_QUARK))

JSClass GomJSWindowClass = {
    "Window", 0,

    JS_PropertyStub, JS_PropertyStub,
    JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub,
    JS_ConvertStub, JS_FinalizeStub
};

static JSPropertySpec gom_js_window_props[] = {
    { NULL }
};

static JSBool
gom_js_window_alert (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
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

typedef struct {
    jsval       fun;
    JSContext  *cx;
    JSObject   *obj;
    guint       gid;
    guint       jsid;
    gboolean    retval;
} GomJSSourceData;

#define SOURCE_MAX (MIN (JSVAL_INT_MAX, G_MAXUINT))

static guint
source_slot (JSContext *cx)
{
    GPtrArray *a;
    guint i;

    a = SOURCES (cx);
    g_assert (a->len <= SOURCE_MAX);
    /* try to reuse old slots every 32 instead of growing? */
    if (!(a->len % 32) || a->len == SOURCE_MAX) {
        for (i = 1; i < a->len; i++) {
            if (!g_ptr_array_index (a, i)) {
                return i;
            }
        }
        if (a->len == SOURCE_MAX) {
            return 0;
        }
    }
    /* if it's 0 and theres an error later it will get reused above */
    g_ptr_array_add (a, NULL);
    return a->len - 1;
}

static void
source_data_free (gpointer data)
{
    GomJSSourceData *sd = data;
    GPtrArray *a = SOURCES (sd->cx);
    g_assert (g_ptr_array_index (a, sd->jsid) == sd);
    g_ptr_array_index (a, sd->jsid) = NULL;
    g_free (sd);
}

static gboolean
source_cb (gpointer data)
{
    GomJSSourceData *sd = data;
    jsval r;

    JS_CallFunctionValue (sd->cx, sd->obj, sd->fun, 0, NULL, &r);

    return sd->retval;
}

static JSBool
_gom_js_window_set_interval (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval, gboolean retval)
{
    GomJSSourceData *data;
    JSFunction *fun = NULL;
    jsval funv = JSVAL_VOID;
    jsdouble dub;
    guint jsid;

    if (argc < 2) {
        return JS_FALSE;
    }

    if (!JS_ValueToNumber (cx, argv[1], &dub)) {
        return JS_FALSE;
    }

    jsid = source_slot (cx);
    if (!jsid) {
        return JS_FALSE;
    }

    if (JSVAL_IS_STRING (argv[0])) {
        char *funstr = JSVAL_CHARS (argv[0]);
        fun = JS_CompileFunction (cx, obj, NULL, 0, NULL,
                                  funstr, strlen (funstr),
                                  NULL, 0);
        if (!fun) {
            return JS_FALSE;
        }
        funv = OBJECT_TO_JSVAL (JS_GetFunctionObject (fun));
    } else if (JSVAL_IS_OBJECT (argv[0]) &&
               JS_ObjectIsFunction (cx, JSVAL_TO_OBJECT (argv[0]))) {
        funv = argv[0];
    } else {
        return JS_FALSE;
    }

    data = g_new0 (GomJSSourceData, 1);

    data->cx  = cx;
    data->obj = obj;
    data->fun = funv;
    data->jsid = jsid;
    data->gid = g_timeout_add_full (G_PRIORITY_LOW, dub, source_cb, data, source_data_free);
    data->retval = retval;

    *rval = INT_TO_JSVAL (jsid);

    g_ptr_array_index (SOURCES (cx), jsid) = data;

    return JS_TRUE;
}

static JSBool
gom_js_window_set_interval (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return _gom_js_window_set_interval (cx, obj, argc, argv, rval, TRUE);
}

static JSBool
gom_js_window_set_timeout (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return _gom_js_window_set_interval (cx, obj, argc, argv, rval, FALSE);
}

static JSBool
_gom_js_window_clear_interval (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval, gboolean retval)
{
    GPtrArray *a = SOURCES (cx);
    GomJSSourceData *sd;
    guint32 jsid;

    if (!JS_ConvertArguments (cx, argc, argv, "u", &jsid)) {
        return JS_FALSE;
    }

    if (jsid >= a->len) {
        g_printerr ("%s:%d:%s(): invalid interval id: %u\n", __FILE__, __LINE__, __FUNCTION__, jsid);
        return JS_FALSE;
    }

    sd = g_ptr_array_index (a, jsid);
    if (!sd || sd->retval != retval) {
        return JS_FALSE;
    }

    g_source_remove (sd->gid);
    return JS_TRUE;
}

static JSBool
gom_js_window_clear_interval (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return _gom_js_window_clear_interval (cx, obj, argc, argv, rval, TRUE);
}

static JSBool
gom_js_window_clear_timeout (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return _gom_js_window_clear_interval (cx, obj, argc, argv, rval, FALSE);
}

static JSBool
gom_js_window_quit (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    gtk_main_quit ();
    return JS_TRUE;
}

static JSFunctionSpec gom_js_window_funcs[] = {
    { "alert",         gom_js_window_alert, 1 },
    { "quit",          gom_js_window_quit,  0 },
    { "setInterval",   gom_js_window_set_interval, 2 },
    { "clearInterval", gom_js_window_clear_interval, 1 },
    { "setTimeout",    gom_js_window_set_timeout, 2 },
    { "lcearTimeout",  gom_js_window_clear_timeout, 1 },
    { NULL }
};

static void
sources_free (gpointer data)
{
    GPtrArray *a = data;
    GomJSSourceData *sd;
    guint i;
    for (i = 0; i < a->len; i++) {
        sd = g_ptr_array_index (a, i);
        if (sd) {
            g_source_remove (sd->gid);
        }
    }
    g_ptr_array_free (data, TRUE);
}

JSObject *
gom_js_window_init_object (JSContext *cx, JSObject *window)
{
    GPtrArray *a = g_ptr_array_new ();
    g_ptr_array_add (a, NULL);
    GOM_JS_CONTEXT_SET_QDATA_FULL (cx, SOURCES_QUARK, a, sources_free);

    JS_DefineProperties (cx, window, gom_js_window_props);
    JS_DefineFunctions (cx, window, gom_js_window_funcs);
    
    JS_DefineProperty (cx, window, "window", OBJECT_TO_JSVAL (window), NULL, NULL,
                       JSPROP_READONLY | JSPROP_PERMANENT);

    return window;
}
