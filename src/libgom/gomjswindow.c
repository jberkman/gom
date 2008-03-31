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

#include "gom/gomjswindow.h"

#include "gom/dom/gomdocument.h"
#include "gom/dom/gomdomimplementation.h"
#include "gom/gomdom.h"
#include "gom/gomjscontext.h"
#include "gom/gomjsexception.h"
#include "gom/gomjsobject.h"
#include "gom/gomobject.h"

#include "gommacros.h"

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

typedef struct {
    const char  *filename;
    JSContext   *cx;
    JSObject    *window;
    GomDocument *doc;
    JSObject    *jsdoc;
    GSList      *elems;
    GString     *script;
} ParserData;

static void
gom_js_window_parser_start_script (GMarkupParseContext *context,
                                   const gchar         *element_name,
                                   const gchar        **attribute_names,
                                   const gchar        **attribute_values,
                                   gpointer             user_data,
                                   GError             **error)
{
    ParserData *data = user_data;
    char *script;
    gsize script_len;
    jsval rval; 
    JSString *str; 
    const char **name, **value;
    char *file;

    for (name = attribute_names, value = attribute_values; *name; ++name, ++value) {
        if (strcmp (*name, "src")) {
            continue;
        }
        if (g_path_is_absolute (*value)) {
            file = (char *)*value;
        } else {
            char *dir = g_path_get_dirname (data->filename);
            file = g_build_filename (dir, *value, NULL);
            g_free (dir);
        }
        if (!g_file_get_contents (file, &script, &script_len, error)) {
            if (*value == file) {
                g_free (file);
            }
            return;
        }
        
        if (!JS_EvaluateScript(data->cx, data->window,
                               script, script_len,
                               *value, 0, &rval)) {
            if (*value == file) {
                g_free (file);
            }
            g_free (script);
            return;
        }
        if (*value == file) {
            g_free (file);
        }
        g_free (script);
        str = JS_ValueToString(data->cx, rval); 
        g_print ("script result: %s\n", JS_GetStringBytes(str));
        /* Rhino 4th Ed. p. 189: "Any code that does appear between
         * these tags is ignored by browsers that support the src
         * attribute..."
         */
        return;
    }
    data->script = g_string_sized_new (1024);
}

/* Called for open tags <foo bar="baz"> */
static void
gom_js_window_parser_start_element (GMarkupParseContext *context,
                                    const gchar         *element_name,
                                    const gchar        **attribute_names,
                                    const gchar        **attribute_values,
                                    gpointer             user_data,
                                    GError             **error)
{
    JSObject *jsobj = NULL;
    GomElement *elem;
    ParserData *data = user_data;
    GParamSpec *spec;
    const char **name, **value;
    guint signal_id;
    JSFunction *fun;
    int lineno;

    if (!data->doc) {
        GomDOMImplementation *dom;
        dom = g_object_new (GOM_TYPE_DOM, NULL);
        data->doc = gom_dom_implementation_create_document (dom, NULL, element_name, NULL, error);
        if (!data->doc) {
            return;
        }
        data->jsdoc = gom_js_object_get_or_create_js_object (data->cx, data->doc);
        JS_DefineProperty (data->cx, data->window, "document",
                           OBJECT_TO_JSVAL (data->jsdoc), NULL, NULL,
                           JSPROP_PERMANENT | JSPROP_READONLY);
        g_object_get (data->doc, "document-element", &elem, NULL);
    } else {
        if (!strcmp (element_name, "script")) {
            gom_js_window_parser_start_script (context, element_name, attribute_names, attribute_values, user_data, error);
            return;
        }

        elem = gom_document_create_element (data->doc, element_name, error);
    }
    g_print ("start_element: %s -> %p\n", element_name, elem);
    if (!elem) {
        return;
    }

    for (name = attribute_names, value = attribute_values; *name; name++, value++) {
        if (!gom_object_resolve (G_OBJECT (elem), *name, &spec, &signal_id)) {
            gom_element_set_attribute (elem, *name, *value, error);
        } else if (signal_id) {
            if (!jsobj) {
                jsobj = gom_js_object_get_or_create_js_object (data->cx, elem);
                if (!jsobj) {
                    if (!gom_js_exception_get_error (data->cx, error)) {
                        g_set_error (error, GOM_JS_ERROR, GOM_JS_ERROR_UNKNOWN,
                                     "Could not get JSObject for elem %s",
                                     element_name);
                    }
                    return;
                }
            }
            
            g_markup_parse_context_get_position (context, &lineno, NULL);
            fun = JS_CompileFunction (data->cx, data->window, NULL, 0, NULL,
                                      *value, strlen (*value),
                                      data->filename, lineno);
            
            if (!fun) {
                if (!gom_js_exception_get_error (data->cx, error)) {
                    g_set_error (error, GOM_JS_ERROR, GOM_JS_ERROR_UNKNOWN,
                                 "Could not compile function at %s:%d", data->filename, lineno);
                }
                return;
            }
            
            if (!gom_js_object_connect (data->cx, jsobj,
                                        signal_id,
                                        fun)) {
                if (!gom_js_exception_get_error (data->cx, error)) {
                    g_set_error (error, GOM_JS_ERROR, GOM_JS_ERROR_UNKNOWN,
                                 "Could not connect signal '%s' to a %s\n",
                                 &(*name)[2], g_type_name (G_TYPE_FROM_INSTANCE (elem)));
                }
                return;
            }
        } else {
            gom_element_set_attribute (elem, spec->name, *value, error);
        }
        if (*error) {
            return;
        }
    }
    if (data->elems) {
        gom_node_append_child (data->elems->data, GOM_NODE (elem), error);
    }
    data->elems = g_slist_prepend (data->elems, elem);
}

/* Called for close tags </foo> */
static void
gom_js_window_parser_end_element (GMarkupParseContext *context,
                                  const gchar         *element_name,
                                  gpointer             user_data,
                                  GError             **error)
{
    ParserData *data = user_data;

    if (!strcmp (element_name, "script")) {
        jsval rval; 
        JSString *str; 
        int lineno;
        /* src tag */
        if (!data->script) {
            return;
        }
        g_markup_parse_context_get_position (context, &lineno, NULL);
        if (!JS_EvaluateScript(data->cx, data->window,
                               data->script->str, data->script->len,
                               data->filename, lineno, &rval)) {
            if (!gom_js_exception_get_error (data->cx, error)) {
                g_set_error (error, GOM_JS_ERROR, GOM_JS_ERROR_UNKNOWN,
                             "Unknown error encountered while running script at %s:%d\n",
                             data->filename, lineno);
            }
            return;
        }
        str = JS_ValueToString(data->cx, rval); 
        g_print ("script result: %s\n", JS_GetStringBytes(str));
        g_string_free (data->script, TRUE);
        data->script = NULL;
    } else {
        data->elems = g_slist_delete_link (data->elems, data->elems);
    }
}

/* Called for character data */
/* text is not nul-terminated */
static void
gom_js_window_parser_text (GMarkupParseContext *context,
                           const gchar         *text,
                           gsize                text_len,  
                           gpointer             user_data,
                           GError             **error)
{
    ParserData *data = user_data;

    if (data->script) {
        g_string_append_len (data->script, text, text_len);
    }
}

static GMarkupParser gom_js_window_parser = {
    gom_js_window_parser_start_element,
    gom_js_window_parser_end_element,
    gom_js_window_parser_text,
    NULL, NULL
};

JSObject *
gom_js_window_parse_file (JSContext  *cx,
                          JSObject   *window,
                          const char *filename)
{
    ParserData data = { NULL };
    GMarkupParseContext *ctx;
    char *xml;
    gsize xmllen;
    GError *error = NULL;

    if (!g_file_get_contents (filename, &xml, &xmllen, &error)) {
        goto out;
    }

    data.filename = filename;
    data.cx = cx;
    data.window = window;
    
    ctx = g_markup_parse_context_new (&gom_js_window_parser,
                                      G_MARKUP_TREAT_CDATA_AS_TEXT,
                                      &data, NULL);
    if (g_markup_parse_context_parse (ctx, xml, xmllen, &error)) {
        g_markup_parse_context_end_parse (ctx, &error);
    }
    g_markup_parse_context_free (ctx);
    g_free (xml);

out:
    if (error) {
        gom_js_exception_set_error (cx, error);
        g_clear_error (&error);
    }

    return data.jsdoc;
}
