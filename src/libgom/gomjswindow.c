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
#include "gom/dom/gomdocumentevent.h"
#include "gom/dom/gomdomimplementation.h"
#include "gom/dom/gomeventtarget.h"
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

static void
gom_js_window_finalize (JSContext *cx, JSObject *obj)
{
    g_message (G_STRLOC": Finalizing a Window.");
}

JSClass GomJSWindowClass = {
    "Window", 0,

    JS_PropertyStub, JS_PropertyStub,
    JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub,
    JS_ConvertStub,
    gom_js_window_finalize
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
        g_printerr (G_STRLOC": invalid interval id: %u\n", jsid);
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

static JSBool
gom_js_window_log (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    char *s;
    if (!JS_ConvertArguments (cx, argc, argv, "s", &s)) {
        return JS_FALSE;
    }

    g_print ("JAVASCRIPT: %s\n", s);

    return JS_TRUE;
}

static JSFunctionSpec gom_js_window_funcs[] = {
    { "alert",         gom_js_window_alert,          1 },
    { "quit",          gom_js_window_quit,           0 },
    { "setInterval",   gom_js_window_set_interval,   2 },
    { "clearInterval", gom_js_window_clear_interval, 1 },
    { "setTimeout",    gom_js_window_set_timeout,    2 },
    { "clearTimeout",  gom_js_window_clear_timeout,  1 },
    { "log",           gom_js_window_log,            1 },
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

typedef struct _ParserScope ParserScope;
struct _ParserScope {
    ParserScope *parent;
    GHashTable  *namespaces;
    GomElement  *elem;
    int          lineno;
};

typedef struct {
    const char  *filename;
    JSContext   *cx;
    JSObject    *window;
    GomDocument *doc;
    JSObject    *jsdoc;
    ParserScope *scope;
} ParserData;

#define XMLNS_LEN 5

static void
push_scope (ParserData   *data,
            const gchar **attribute_names,
            const gchar **attribute_values)
{
    ParserScope *scope;
    const char **name, **value;

    scope = g_new0 (ParserScope, 1);

    scope->parent = data->scope;
    data->scope = scope;

    scope->namespaces = g_hash_table_new_full (g_str_hash, g_str_equal,
                                               g_free, g_free);
    for (name = attribute_names, value = attribute_values; *name; name++, value++) {
        if (!strcmp (*name, "xmlns")) {
            g_print (G_STRLOC": default namespace: %s\n", *value);
            g_hash_table_replace (scope->namespaces, 
                                  g_strdup (""),
                                  g_strdup (*value));
        } else if (!strncmp (*name, "xmlns:", XMLNS_LEN+1)) {
            g_print (G_STRLOC": found prefix: %s -> %s\n",
                     *name+XMLNS_LEN+1,
                     *value);
            g_hash_table_replace (scope->namespaces,
                                  g_strdup (*name + XMLNS_LEN + 1),
                                  g_strdup (*value));
        }
    }
}

static void
pop_scope (ParserData *data)
{
    ParserScope *scope;

    scope = data->scope;
    if (!scope) {
        return;
    }
    data->scope = scope->parent;

    g_hash_table_destroy (scope->namespaces);
    if (scope->elem) {
        g_object_unref (scope->elem);
    }
    g_free (scope);
}

static const char *
lookup_namespace_prefix (ParserScope *scope, const char *prefix)
{
    const char *ns;
    ns = g_hash_table_lookup (scope->namespaces, prefix);
    if (!ns && scope->parent) {
        ns = lookup_namespace_prefix (scope->parent, prefix);
    }
    return ns;
}

static const char *
lookup_namespace_qualified (ParserData *data, const char *qualified_name)
{
    const char *namespace;
    const char *colon;
    char *prefix = NULL;

    colon = strchr (qualified_name, ':');
    prefix = colon ? g_strndup (qualified_name, colon - qualified_name) : NULL;
    namespace = lookup_namespace_prefix (data->scope, prefix ? prefix : "");
    g_free (prefix);
    return namespace;
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
    ParserData *data = user_data;
    GParamSpec *spec;
    const char **name, **value;
    const char *namespace;
    guint signal_id;
    JSFunction *fun;
    int lineno;

    push_scope (data, attribute_names, attribute_values);
    g_markup_parse_context_get_position (context, &data->scope->lineno, NULL);
    namespace = lookup_namespace_qualified (data, element_name);

    if (!data->doc) {
        GomDOMImplementation *dom;
        dom = g_object_new (GOM_TYPE_DOM, NULL);
        data->doc = gom_dom_implementation_create_document (dom, namespace, element_name, NULL, error);
        if (data->doc) {
            data->jsdoc = gom_js_object_get_or_create_js_object (data->cx, data->doc);
            JS_DefineProperty (data->cx, data->window, "document",
                               OBJECT_TO_JSVAL (data->jsdoc), NULL, NULL,
                               JSPROP_PERMANENT | JSPROP_READONLY);
            g_object_get (data->doc, "document-element", &data->scope->elem, NULL);
        }
    } else {
        data->scope->elem = gom_document_create_element_ns (data->doc, namespace, element_name, error);
    }

#if 0
    g_print ("start_element: %s -> %p\n", element_name, data->scope->elem);
#endif

    for (name = attribute_names, value = attribute_values; 
         !*error && *name;
         name++, value++) {
        gom_object_resolve (G_OBJECT (data->scope->elem), *name, &spec, &signal_id);
        if (signal_id) {
            if (!jsobj) {
                jsobj = gom_js_object_get_or_create_js_object (data->cx, data->scope->elem);
                if (!jsobj) {
                    if (!gom_js_exception_get_error (data->cx, error)) {
                        g_set_error (error, GOM_JS_ERROR, GOM_JS_ERROR_UNKNOWN,
                                     "Could not get JSObject for elem %s",
                                     element_name);
                    }
                    break;
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
                break;
            }
            
            if (!gom_js_object_connect (data->cx, jsobj,
                                        signal_id,
                                        fun)) {
                if (!gom_js_exception_get_error (data->cx, error)) {
                    g_set_error (error, GOM_JS_ERROR, GOM_JS_ERROR_UNKNOWN,
                                 "Could not connect signal '%s' to a %s\n",
                                 &(*name)[2], g_type_name (G_TYPE_FROM_INSTANCE (data->scope->elem)));
                }
                break;
            }
        } else {
            /* in the spec case, this drops the prefix that *name
             * might have */
            gom_element_set_attribute_ns (data->scope->elem, 
                                          lookup_namespace_qualified (data, *name),
                                          spec ? spec->name : *name, *value,
                                          error);
        }
    }
    if (!*error && data->scope->parent) {
        gom_node_append_child (GOM_NODE (data->scope->parent->elem),
                               GOM_NODE (data->scope->elem),
                               error);
    }
    if (*error) {
        pop_scope (data);
    }
}

/* Called for close tags </foo> */
static void
gom_js_window_parser_end_element (GMarkupParseContext *context,
                                  const gchar         *element_name,
                                  gpointer             user_data,
                                  GError             **error)
{
    ParserData *data = user_data;
    char       *local_name = NULL;

    g_object_get (data->scope->elem, "local-name", &local_name, NULL);
    element_name = local_name;

    if (!strcmp (element_name, "script")) {
        char *file = NULL;

        char *script = NULL;
        gsize script_len;

        GError *err = NULL;

        const char *filename;
        int lineno;
        
        /* Rhino 4th Ed. p. 189: "Any code that does appear between
         * these tags is ignored by browsers that support the src
         * attribute..."
         */
        file = gom_element_get_attribute (data->scope->elem, "src");
        if (file) {
            if (!g_path_is_absolute (file)) {
                char *dir = g_path_get_dirname (data->filename);
                char *f2  = g_build_filename (dir, file, NULL);
                g_free (file);
                g_free (dir);
                file = f2;
            }
            if (!g_file_get_contents (file, &script, &script_len, &err)) {
                g_printerr (G_STRLOC": could not load %s: %s\n",
                            file, err->message);
                g_error_free (err);
            }
            filename = file;
            lineno   = 0;
        } else {
            GomNodeList *children;
            gom_node_normalize (GOM_NODE (data->scope->elem));
            g_object_get (data->scope->elem, "child-nodes", &children, NULL);
            if (children) {
                gulong length, i;
                GomNode *child;
                GString *str = g_string_sized_new (1024);
                g_object_get (children, "length", &length, NULL);
                for (i = 0; i < length; i++) {
                    child = gom_node_list_item (children, i);
                    if (GOM_IS_TEXT (child)) {
                        script = NULL;
                        g_object_get (child, "data", &script, NULL);
                        g_string_append (str, script);
                        g_free (script);
                    }
                }
                g_object_unref (children);
                script_len = str->len;
                script = g_string_free (str, FALSE);
            }
            filename = data->filename;
            lineno   = data->scope->lineno;
        }
        if (script) {
            jsval rval; 
            JSString *str; 
            if (!JS_EvaluateScript(data->cx, data->window,
                                   script, script_len,
                                   filename, lineno, &rval)) {
                if (!gom_js_exception_get_error (data->cx, &err)) {
                    g_set_error (error, GOM_JS_ERROR, GOM_JS_ERROR_UNKNOWN,
                                 "Unknown error encountered while running script at %s:%d\n",
                                 data->filename, lineno);
                }
                g_printerr (G_STRLOC": error running script: %s\n", err->message);
                g_error_free (err);
            } else {
                str = JS_ValueToString(data->cx, rval); 
                g_print (G_STRLOC": script result: %s\n", 
                         JS_GetStringBytes(str));
            }
        }
        g_free (script);
        g_free (file);
    }
    g_free (local_name);
    pop_scope (data);
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
    GomText *text_node;
    char *txt;

    if (!data->doc || !data->scope) {
        g_print (G_STRLOC": dropping some text...\n");
        return;
    }

    /* sigh, c strings... */
    txt = g_strndup (text, text_len);
    text_node = gom_document_create_text_node (data->doc, txt);
    gom_node_append_child (
        GOM_NODE (data->scope->elem),
        GOM_NODE (text_node),
        error);
    g_object_unref (text_node);
    g_free (txt);
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
    if (data.doc) {
        g_object_unref (data.doc);
    }

out:
    if (error) {
        gom_js_exception_set_error (cx, &error);
    }

    return data.jsdoc;
}
