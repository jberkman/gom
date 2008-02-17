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

#include <gom/gomdoc.h>

#include <gom/gomjsdocument.h>
#include <gom/gomjsobject.h>

#include <glib/gmarkup.h>

#include <gtk/gtkwidget.h>

#include <string.h>

typedef struct {
    GList *children;
} GomDocPrivate;

#define PRIV(o) G_TYPE_INSTANCE_GET_PRIVATE ((o), GOM_TYPE_DOC, GomDocPrivate)

/* GomNodeInterface */
/* attributes */
static G_CONST_RETURN char *
gom_doc_get_node_name (GomNode *node)
{
    return "document"; /* ??? */
}

static char *
gom_doc_get_node_value (GomNode *node, GError **error)
{
    return NULL;
}

static void
gom_doc_set_node_value (GomNode *node, const char *value, GError **error)
{
}

static GomNodeType
gom_doc_get_node_type (GomNode *node)
{
    return GOM_DOCUMENT_NODE;
}

static GomNode *
gom_doc_get_parent_node (GomNode *node)
{
    return NULL;
}

static GomNodeList *
gom_doc_get_child_nodes (GomNode *node)
{
    return NULL;
}

static GomNode *
gom_doc_get_first_child (GomNode *node)
{
    GomDocPrivate *priv = PRIV (node);

    return priv->children->data;
}

static GomNode *
gom_doc_get_last_child (GomNode *node)
{
    return NULL;
}

static GomNode *
gom_doc_get_previous_sibling (GomNode *node)
{
    return NULL;
}

static GomNode *
gom_doc_get_next_sibling (GomNode *node)
{
    return NULL;
}

static GomNamedNodeMap *
gom_doc_get_attributes (GomNode *node)
{
    return NULL;
}

static GomDocument *
gom_doc_get_owner_document (GomNode *node)
{
    return NULL;
}

static GomNode *
gom_doc_insert_before (GomNode *node,
                      GomNode *new_child,
                      GomNode *ref_child,
                      GError  **error)
{
    return NULL;
}

static GomNode *
gom_doc_replace_child (GomNode *node,
                      GomNode *new_child,
                      GomNode *ref_child,
                      GError  **error)
{
    return NULL;
}

static GomNode *
gom_doc_remove_child (GomNode *node,
                     GomNode *old_child,
                     GError  **error)
{
    return NULL;
}

static GomNode *
gom_doc_append_child (GomNode *node,
                      GomNode *new_child,
                      GError  **error)
{
    GomDocPrivate *priv = PRIV (node);

    priv->children = g_list_append (priv->children, new_child);

    return NULL;
}

static gboolean
gom_doc_has_child_nodes (GomNode *node)
{
    GomDocPrivate *priv = PRIV (node);

    return g_list_length (priv->children) != 0;
}

static GomNode *
gom_doc_clone_node (GomNode *node, gboolean deep)
{
    return NULL;
}

static void
gom_doc_node_init (gpointer g_iface, gpointer iface_data)
{
    GomNodeInterface *node = (GomNodeInterface *)g_iface;

#define IFACE(func) node->func = gom_doc_##func

    IFACE (get_node_name);
    IFACE (get_node_value);
    IFACE (set_node_value);
    IFACE (get_node_type);
    IFACE (get_parent_node);
    IFACE (get_child_nodes);
    IFACE (get_first_child);
    IFACE (get_last_child);
    IFACE (get_previous_sibling);
    IFACE (get_next_sibling);
    IFACE (get_attributes);
    IFACE (get_owner_document);

    IFACE (insert_before);
    IFACE (replace_child);
    IFACE (remove_child);
    IFACE (append_child);
    IFACE (has_child_nodes);
    IFACE (clone_node);

#undef IFACE
}

/* GomDocumentInterface */
/* attributes */
static GomDocumentType *
gom_doc_get_doctype (GomDocument *doc)
{
    return NULL;
}

static GomDOMImplementation *
gom_doc_get_implementation (GomDocument *doc)
{
    return NULL;
}

static GomElement *
gom_doc_get_document_element (GomDocument *doc)
{
    return NULL;
}

/* methods */
static GomElement *
gom_doc_create_element (GomDocument *doc,
                        const char  *tag_name,
                        GError     **error)
{
    GType type;
    GObject *obj = NULL;

    type = g_type_from_name (tag_name);
    if (type != 0 && g_type_is_a (type, GOM_TYPE_ELEMENT)) {
        obj = g_object_new (type, NULL);
    }
    if (GTK_IS_WIDGET (obj)) {
        gtk_widget_show (GTK_WIDGET (obj));
    }

    return obj ? GOM_ELEMENT (obj) : NULL;
}

static GomDocumentFragment *
gom_doc_create_document_fragment (GomDocument *doc)
{
    return NULL;
}

static GomText *
gom_doc_create_text_node (GomDocument *doc,
                          const char  *data)
{
    return NULL;
}

static GomComment *
gom_doc_create_comment (GomDocument *doc,
                        const char  *data)
{
    return NULL;
}

static GomCDATASection *
gom_doc_create_cdata_section (GomDocument *doc,
                              const char  *data,
                              GError     **error)
{
    return NULL;
}

static GomProcessingInstruction *
gom_doc_create_processing_instruction (GomDocument *doc,
                                       const char  *target,
                                       const char  *data,
                                       GError     **error)
{
    return NULL;
}

static GomAttr *
gom_doc_create_attribute (GomDocument *doc,
                          const char  *name,
                          GError     **error)
{
    return NULL;
}

static GomEntityReference *
gom_doc_create_entity_reference (GomDocument *doc,
                                 const char  *name,
                                 GError     **error)
{
    return NULL;
}

static GomNodeList *
gom_doc_get_elements_by_tag_name (GomDocument *doc,
                                  const char  *tagname)
{
    return NULL;
}

static void
gom_doc_document_init (gpointer g_iface, gpointer iface_data)
{
    GomDocumentInterface *iface = (GomDocumentInterface *)g_iface;

#define IFACE(func) iface->func = gom_doc_##func

    /* attributes */
    IFACE (get_doctype);
    IFACE (get_implementation);
    IFACE (get_document_element);

    /* methods */
    IFACE (create_element);
    IFACE (create_document_fragment);
    IFACE (create_text_node);
    IFACE (create_comment);
    IFACE (create_cdata_section);
    IFACE (create_processing_instruction);
    IFACE (create_attribute);
    IFACE (create_entity_reference);
    IFACE (get_elements_by_tag_name);
#undef IFACE
}

GOM_DEFINE_JS_OBJECT (gom_doc, GomJSDocumentClass)

G_DEFINE_TYPE_WITH_CODE (GomDoc, gom_doc, G_TYPE_OBJECT,
                         {
                             G_IMPLEMENT_INTERFACE (GOM_TYPE_NODE, gom_doc_node_init);
                             G_IMPLEMENT_INTERFACE (GOM_TYPE_DOCUMENT, gom_doc_document_init);
                             G_IMPLEMENT_INTERFACE (GOM_TYPE_JS_OBJECT, gom_doc_js_object_init);
                         });

static void gom_doc_init (GomDoc *doc) { }

static void
gom_doc_class_init (GomDocClass *klass)
{
    g_type_class_add_private (klass, sizeof (GomDocPrivate));
}

typedef struct {
    const char *filename;
    JSContext *cx;
    JSObject *jsobj;
    GomDocument *doc;
    GSList *elems;
    GString *script;
} GomDomParserData;

/* Called for open tags <foo bar="baz"> */
static void
gom_dom_parser_start_element (GMarkupParseContext *context,
                              const gchar         *element_name,
                              const gchar        **attribute_names,
                              const gchar        **attribute_values,
                              gpointer             user_data,
                              GError             **error)
{
    GomElement *elem;
    GomDomParserData *data = user_data;
    int i;

    if (!strcmp (element_name, "script")) {
        data->script = g_string_sized_new (1024);
    } else {
        elem = gom_document_create_element (data->doc, element_name, error);
        g_print ("start_element: %s -> %p\n", element_name, elem);
        if (elem) {
            for (i = 0; attribute_names[i]; i++) {
                if (attribute_names[i][0] == 'o' && 
                    attribute_names[i][1] == 'n' && 
                    g_signal_lookup (&attribute_names[i][2], G_TYPE_FROM_INSTANCE (elem))) {
                    JSObject *jsobj;
                    JSFunction *fun;
                    int lineno;

                    jsobj = gom_js_object_get_or_create_js_object (data->cx, elem);
                    if (!jsobj) {
                        g_printerr ("coudn't get jsobj for %s\n", element_name);
                        continue;
                    }

                    g_markup_parse_context_get_position (context, &lineno, NULL);
                    fun = JS_CompileFunction (data->cx, data->jsobj, NULL, 0, NULL,
                                              attribute_values[i], strlen (attribute_values[i]),
                                              data->filename, lineno);

                    if (!fun) {
                        g_printerr ("couldn't compile script: %s\n", attribute_values[i]);
                        continue;
                    }

                    if (!gom_js_object_connect (data->cx, jsobj,
                                                &attribute_names[i][2],
                                                fun)) {
                        g_printerr ("couldn't connect script for signal %s\n", attribute_names[i]);
                        continue;
                    }
                } else {
                    gom_element_set_attribute (elem, attribute_names[i], attribute_values[i], error);
                }
                if (error && *error) {
                    return;
                }
            }
            data->elems = g_slist_prepend (data->elems, elem);
        }
    }
}

/* Called for close tags </foo> */
static void
gom_dom_parser_end_element (GMarkupParseContext *context,
                            const gchar         *element_name,
                            gpointer             user_data,
                            GError             **error)
{
    GomDomParserData *data = user_data;

    if (!strcmp (element_name, "script")) {
        jsval rval; 
        JSString *str; 
        JSBool ok;
        int lineno;

        g_markup_parse_context_get_position (context, &lineno, NULL);
        ok = JS_EvaluateScript(data->cx, data->jsobj,
                               data->script->str, data->script->len,
                               data->filename, lineno, &rval);
        str = JS_ValueToString(data->cx, rval); 
        g_print ("script result: %s\n", JS_GetStringBytes(str));
        g_string_free (data->script, TRUE);
        data->script = NULL;
    } else {
        GomNode *parent;
        GomNode *elem;

        elem = data->elems->data;
        data->elems = g_slist_delete_link (data->elems, data->elems);
        
        parent = data->elems ? data->elems->data : data->doc;
        
        g_print ("end_element: %s -> %p => %p\n", element_name, elem, parent);

        gom_node_append_child (parent, elem, error);
    }
}

/* Called for character data */
/* text is not nul-terminated */
static void
gom_dom_parser_text (GMarkupParseContext *context,
                     const gchar         *text,
                     gsize                text_len,  
                     gpointer             user_data,
                     GError             **error)
{
    GomDomParserData *data = user_data;

    if (data->script) {
        g_string_append_len (data->script, text, text_len);
    } else {
        g_print ("ignoring some text: %.*s\n", (int)text_len, text);
    }
}

/* Called on error, including one set by other
 * methods in the vtable. The GError should not be freed.
 */
static void
gom_dom_parser_error (GMarkupParseContext *context,
                      GError              *error,
                      gpointer             user_data)
{
    g_print ("oh, we got a parse error\n");
}

static GMarkupParser gom_dom_parser = {
    gom_dom_parser_start_element,
    gom_dom_parser_end_element,
    gom_dom_parser_text,
    NULL,
    gom_dom_parser_error
};

gboolean
gom_doc_parse_file (GomDocument *doc,
                    JSContext *cx, JSObject *jsobj,
                    const char *filename,
                    GError **error)
{
    GomDomParserData data;
    GMarkupParseContext *ctx;
    char *xml;
    gsize xmllen;
    gboolean ret;

    if (!g_file_get_contents (filename, &xml, &xmllen, error)) {
        return FALSE;
    }

    data.filename = filename;
    data.cx = cx;
    data.jsobj = jsobj;
    data.doc = doc;
    data.elems = NULL;
    data.script = NULL;
    
    ctx = g_markup_parse_context_new (&gom_dom_parser, G_MARKUP_TREAT_CDATA_AS_TEXT, &data, NULL);
    ret = g_markup_parse_context_parse (ctx, xml, xmllen, error);
    if (ret) {
        ret = g_markup_parse_context_end_parse (ctx, error);
    }
    g_markup_parse_context_free (ctx);
    g_free (xml);

    return ret;
}
