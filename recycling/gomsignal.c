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

#include <gom/gomsignal.h>

#include <gom/gomwidget.h>

#include <gtk/gtk.h>

#include <string.h>

static GHashTable *signals = NULL;

typedef struct {
    GType    type;
    gboolean pointer : 1;
} SomeType;

typedef struct {
    SomeType  return_type;
    guint     n_params;
    SomeType *param_types;
} SignalNode;

typedef struct {
    GType type;
    guint signal;
    SignalNode *node;
} SignalParserData;

GQuark
gom_signal_error_quark (void)
{
    return g_quark_from_static_string ("gom-signal-error-quark");
}

static void
free_node (gpointer data)
{
    SignalNode *node = data;
    g_free (node->param_types);
    g_free (node);
}

static const char *
get_value (const char *name, const char **attribute_names, const char **attribute_values)
{
   int i;
    for (i = 0; attribute_names[i]; i++) {
        if (!strcmp (attribute_names[i], name)) {
            return attribute_values[i];
        }
    }
    return NULL;
}

static void
start_object (GMarkupParseContext *context,
              const gchar         *element_name,
              const gchar        **attribute_names,
              const gchar        **attribute_values,
              gpointer             user_data,
              GError             **error)
{
    SignalParserData *data = user_data;
    const char *cname;

    cname = get_value ("cname", attribute_names, attribute_values);
    if (!cname) {
        g_set_error (error, GOM_SIGNAL_ERROR,
                     GOM_SIGNAL_ERROR_MISSING_CNAME_ATTRIBUTE,
                     "missing \"cname\" attribute for <%s>",
                     element_name);
        return;
    }
    data->type = g_type_from_name (cname);
    if (!data->type) {
        /* a bunch of these are for "static" fields and it's ok to "fail" silently here */
        return;
    }
}

static void
start_signal (GMarkupParseContext *context,
              const gchar         *element_name,
              const gchar        **attribute_names,
              const gchar        **attribute_values,
              gpointer             user_data,
              GError             **error)
{
    SignalParserData *data = user_data;
    const char *cname;

    cname = get_value ("cname", attribute_names, attribute_values);
    if (!cname) {
        g_set_error (error, GOM_SIGNAL_ERROR,
                     GOM_SIGNAL_ERROR_MISSING_CNAME_ATTRIBUTE,
                     "missing \"cname\" attribute for <%s>",
                     element_name);
        return;
    }
    data->signal = g_signal_lookup (cname, g_type_from_name (data->type));
    if (!data->signal) {
        g_set_error (error, GOM_SIGNAL_ERROR,
                     GOM_SIGNAL_ERROR_SIGNAL_NOT_FOUND,
                     "GType \"%s\" has no signal named \"%s\"",
                     g_type_name (data->type), cname);
        return;
    }
    data->node = g_new0 (SignalNode, 1);
}

static void
get_type (SomeType *atype,
          const char   *element_name,
          const gchar **attribute_names,
          const gchar **attribute_values,
          GError      **error)
{
    SignalParserData *data = user_data;
    const char *type;
    const char *pointer;
    char *pointer_type;

    type = get_value ("type", attribute_names, attribute_values);
    if (!type) {
        g_set_error (error,
                     GOM_SIGNAL_ERROR,
                     GOM_SIGNAL_ERROR_MISSING_TYPE_ATTRIBUTE,
                     "signal \"%s\" for GType \"%s\" has a missing \"type\" attribute for <%s>",
                     g_signal_name (data->signal), 
                     g_type_name (data->type),
                     element_name);
        return;
    }

    pointer = strchr (type, '*');
    if (pointer) {
        if (strchr (pointer + 1, '*')) {
            g_set_error (error,
                         GOM_SIGNAL_ERROR,
                         GOM_SIGNAL_ERROR_POINTERS_TO_POINTERS_UNSUPPORTED,
                         "signal \"%s\" for GType \"%s\" has a \"type\" of \"%s\" for <%s>; "
                         "these are not currently supported by Gom",
                         g_signal_name (data->signal), 
                         g_type_name (data->type),
                         element_name);
            return;
        }
        pointer_type = g_strndup (type, pointer - type);
        atype->pointer = TRUE;
        atype->type = g_type_from_name (pointer_type);
        g_free (pointer_type);
    } else {
        atype->type = g_type_from_name (type);
    }
}

/* Called for open tags <foo bar="baz"> */
static void
signal_parser_start_element (GMarkupParseContext *context,
                              const gchar         *element_name,
                              const gchar        **attribute_names,
                              const gchar        **attribute_values,
                              gpointer             user_data,
                              GError             **error)
{
    SignalParserData *data = user_data;
    if (data->type) {
        if (data->signal) {
            if (!strcmp (element_name, "return-type")) {
                get_type (&data->signal->return_type, element_name, attribute_names, attribute_values, error);
            } else if (!strcmp (element_name, "parameter")) {
                ++data->node->n_params;
                data->node->param_types = g_renew (SomeType, data->node->param_types, data->node->n_params);
                get_type (&data->node->param_types[data->node->n_params - 1],
                          element_name, attribute_names, attribute_values, error);
            }
        } else if (!strcmp (element_name, "signal")) {
            start_signal (context, element_name, attribute_names, attribute_values, user_data, error);
        }
    } else if (!strcmp (element_name, "object") ||
               !strcmp (element_name, "interface")) {
        start_object (context, element_name, attribute_names, attribute_values, user_data, error);
    }
}

/* Called for close tags </foo> */
static void
signal_parser_end_element (GMarkupParseContext *context,
                            const gchar         *element_name,
                            gpointer             user_data,
                            GError             **error)
{
    SignalParserData *data = user_data;
    if (data->type) {
        if (data->signal) {
            if (!strcmp (element_name, "signal")) {
                g_print ("adding %s::%s\n", g_type_name (data->type), g_signal_name (data->signal));
                g_hash_table_insert (signals, GINT_TO_POINTER (data->signal), data->node);
                data->signal = NULL;
                data->node = NULL;
            }
        } else if (!strcmp (element_name, "object") ||
                   !strcmp (element_name, "interface")) {
            data->type = NULL;
        }
    }
}

static GMarkupParser signal_parser = {
    signal_parser_start_element,
    signal_parser_end_element,
    NULL, NULL, NULL
};

gboolean
gom_signal_init (GError **error)
{
    gboolean ret;
    char *api;
    gsize apilen;
    GMarkupParseContext *cx;
    SignalParserData data = { NULL };

    if (!g_file_get_contents ("gtk-api.raw", &api, &apilen, &error)) {
        return FALSE;
    }
    
    signals = g_hash_table_new_full (g_int_hash, g_int_equal, NULL, free_node);

    cx = g_markup_parse_context_new (&signal_parser, 0, &data, NULL);
    ret = g_markup_parse_context_parse (cx, script, scriptlen, &error);
    if (ret) {
        ret = g_markup_parse_context_end_parse (cx, &error);
    }

    free_node (data.signal);

    g_markup_parse_context_free (cx);

    return ret;
}

typedef struct {
    JSContext  *cx;
    JSScript   *script;
    JSObject   *scriptobj;
    SignalNode *node;
} SignalData;

static void


gulong
gom_signal_connect_script (JSContext *cx, JSObject *obj,
                           const char *signal,
                           const char *script, gsize scriptlen,
                           GError **error)
{
    SignalData *data = NULL;
    GObject *gobj;
    guint signal_id;
    GCallback c_handler;

    gobj = gom_js_object_get_g_object (cx, obj);
    if (!gobj) {
        g_printerr ("Could not find GObject for JSObject %p\n", obj);
        return 0;
    }
    signal_id = g_signal_lookup (signal, G_TYPE_FROM_INSTANCE (gobj));
    if (!signal_id) {
        g_printerr ("Could not find signal %s for %s %p\n",
                    signal, g_type_name (G_TYPE_FROM_INSTANCE (gobj)),
                    gobj);
        return;
    }

    data = g_new0 (SignalData, 1);

    data->node = g_hash_table_lookup (signals, GINT_TO_POINTER (signal_id));
    if (!data->node) {
        g_printerr ("Could not find signal node for signal %s on %s %p\n",
                    signal, g_type_name (G_TYPE_FROM_INSTANCE (gobj)),
                    gobj);
        goto script_fail;
    }

    data->script = JS_CompileScript (cx, obj, script, scriptlen, "<script">, 0);
    if (!data->script) {
        g_perinterr ("Error compiling script '%.*s'\n", script, scriptlen);
        goto script_fail;
    }

    data->scriptobj = JS_NewScriptObject (cx, data->script);
    if (!data->scriptobj || !JS_AddRoot (cx, &data->scriptobj)) {
        goto script_fail;
    }


}

