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

#include <gommacros.h>

#include <gom/dom/gomdomexception.h>
#include <gom/gomglist.h>
#include <gom/gomjsdocument.h>
#include <gom/gomjsexception.h>
#include <gom/gomjsobject.h>
#include <gom/gomobject.h>

#include <glib/gmarkup.h>

#include <gtk/gtkwidget.h>

#include <string.h>

enum {
    PROP_NODE_NAME = 1,
    PROP_NODE_VALUE,
    PROP_NODE_TYPE,
    PROP_PARENT_NODE,
    PROP_CHILD_NODES,
    PROP_FIRST_CHILD,
    PROP_LAST_CHILD,
    PROP_PREVIOUS_SIBLING,
    PROP_NEXT_SIBLING,
    PROP_ATTRIBUTES,
    PROP_OWNER_DOCUMENT,

    PROP_DOCTYPE,
    PROP_IMPLEMENTATION,
    PROP_DOCUMENT_ELEMENT
};

typedef struct {
    GList *children;
} GomDocPrivate;

#define PRIV(o) G_TYPE_INSTANCE_GET_PRIVATE ((o), GOM_TYPE_DOC, GomDocPrivate)

GOM_DEFINE_QUARK(doc_error)

/* GomNodeInterface */
/* GomDocumentInterface */
static void
gom_doc_get_property (GObject    *object,
                      guint       property_id,
                      GValue     *value,
                      GParamSpec *pspec)
{
    GomDocPrivate *priv = PRIV (object);

    switch (property_id) {
    case PROP_NODE_NAME:
        g_value_set_static_string (value, "document");
        break;
    case PROP_NODE_TYPE:
        g_value_set_uint (value, GOM_DOCUMENT_NODE);
        break;
    case PROP_OWNER_DOCUMENT:
    case PROP_PREVIOUS_SIBLING:
    case PROP_NEXT_SIBLING:
    case PROP_PARENT_NODE:
        g_value_set_object (value, NULL);
        break;
    case PROP_CHILD_NODES:
        g_value_set_object (value, priv->children ? gom_g_list_new (priv->children) : NULL);
        break;
    case PROP_FIRST_CHILD:
        g_value_set_object (value, priv->children ? priv->children->data : NULL);
        break;
    case PROP_LAST_CHILD: {
        GList *last;
        last = g_list_last (priv->children);
        g_value_set_object (value, last ? last->data : NULL);
        break;
    }
    case PROP_NODE_VALUE:
    case PROP_ATTRIBUTES:
    case PROP_DOCTYPE:
    case PROP_IMPLEMENTATION:
    case PROP_DOCUMENT_ELEMENT:
        GOM_NOT_IMPLEMENTED;
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
gom_doc_set_property (GObject *object,
                      guint property_id,
                      const GValue *value,
                      GParamSpec *pspec)
{
    switch (property_id) {
    case PROP_NODE_VALUE:
        GOM_NOT_IMPLEMENTED;
        break;
    case PROP_NODE_TYPE:
    case PROP_PARENT_NODE:
    case PROP_CHILD_NODES:
    case PROP_FIRST_CHILD:
    case PROP_LAST_CHILD:
    case PROP_PREVIOUS_SIBLING:
    case PROP_NEXT_SIBLING:
    case PROP_ATTRIBUTES:
    case PROP_OWNER_DOCUMENT:
    case PROP_DOCTYPE:
    case PROP_IMPLEMENTATION:
    case PROP_DOCUMENT_ELEMENT:
        g_assert_not_reached ();
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static GomNode *
gom_doc_insert_before (GomNode *node,
                      GomNode *new_child,
                      GomNode *ref_child,
                      GError  **error)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomNode *
gom_doc_replace_child (GomNode *node,
                      GomNode *new_child,
                      GomNode *ref_child,
                      GError  **error)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomNode *
gom_doc_remove_child (GomNode *node,
                     GomNode *old_child,
                     GError  **error)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomNode *
gom_doc_append_child (GomNode *node,
                      GomNode *new_child,
                      GError  **error)
{
    GomDocPrivate *priv = PRIV (node);

    priv->children = g_list_append (priv->children, new_child);

    return new_child;
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
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static void
gom_doc_node_init (gpointer g_iface, gpointer iface_data)
{
    GomNodeInterface *node = (GomNodeInterface *)g_iface;

#define IFACE(func) node->func = gom_doc_##func

    IFACE (insert_before);
    IFACE (replace_child);
    IFACE (remove_child);
    IFACE (append_child);
    IFACE (has_child_nodes);
    IFACE (clone_node);

#undef IFACE
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
    if (!obj) {
        g_set_error (error,
                     GOM_DOM_EXCEPTION_ERROR,
                     GOM_DOM_EXCEPTION_ERROR_UNKNOWN_TAG_NAME,
                     "%s is not a registered GType",
                     tag_name);
        return NULL;
    }

    if (GTK_IS_WIDGET (obj)) {
        gtk_widget_show (GTK_WIDGET (obj));
    }

    return GOM_ELEMENT (obj);
}

static GomDocumentFragment *
gom_doc_create_document_fragment (GomDocument *doc)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomText *
gom_doc_create_text_node (GomDocument *doc,
                          const char  *data)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomComment *
gom_doc_create_comment (GomDocument *doc,
                        const char  *data)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomCDATASection *
gom_doc_create_cdata_section (GomDocument *doc,
                              const char  *data,
                              GError     **error)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomProcessingInstruction *
gom_doc_create_processing_instruction (GomDocument *doc,
                                       const char  *target,
                                       const char  *data,
                                       GError     **error)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomAttr *
gom_doc_create_attribute (GomDocument *doc,
                          const char  *name,
                          GError     **error)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomEntityReference *
gom_doc_create_entity_reference (GomDocument *doc,
                                 const char  *name,
                                 GError     **error)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomNodeList *
gom_doc_get_elements_by_tag_name (GomDocument *doc,
                                  const char  *tag_name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomElement *
element_get_element_by_id (GomElement *elem,
                           const char *element_id)
{
    GValue *gval;
    GomNode *node;
    GomElement *ret;

    gval = gom_object_get_attribute (G_OBJECT (elem), "id");
    if (gval && G_VALUE_HOLDS_STRING (gval) && !strcmp (g_value_get_string (gval), element_id)) {
        return elem;
    }

    for (g_object_get (elem, "first-child", &node, NULL); node; g_object_get (node, "next-sibling", &node, NULL)) {
        if (GOM_IS_ELEMENT (node)) {
            ret = element_get_element_by_id (GOM_ELEMENT (node), element_id);
            if (ret) {
                return ret;
            }
        }
    }

    return NULL;
}

/* Introduced in DOM Level 2: */
static GomElement *
gom_doc_get_element_by_id (GomDocument *doc,
                           const char *element_id)
{
    GomDocPrivate *priv = PRIV (doc);
    GomElement *elem;
    GList *li;

    for (li = priv->children; li; li = li->next) {
        elem = element_get_element_by_id (GOM_ELEMENT (li->data), element_id);
        if (elem) {
            return elem;
        }
    }
    return NULL;
}

static void
gom_doc_document_init (gpointer g_iface, gpointer iface_data)
{
    GomDocumentInterface *iface = (GomDocumentInterface *)g_iface;

#define IFACE(func) iface->func = gom_doc_##func

    IFACE (create_element);
    IFACE (create_document_fragment);
    IFACE (create_text_node);
    IFACE (create_comment);
    IFACE (create_cdata_section);
    IFACE (create_processing_instruction);
    IFACE (create_attribute);
    IFACE (create_entity_reference);
    IFACE (get_elements_by_tag_name);

    /* Introduced in DOM Level 2: */
    IFACE (get_element_by_id);
#undef IFACE
}

G_DEFINE_TYPE_WITH_CODE (GomDoc, gom_doc, G_TYPE_OBJECT,
                         {
                             G_IMPLEMENT_INTERFACE (GOM_TYPE_NODE, gom_doc_node_init);
                             G_IMPLEMENT_INTERFACE (GOM_TYPE_DOCUMENT, gom_doc_document_init);
                         });

static void gom_doc_init (GomDoc *doc) { }

static void
gom_doc_class_init (GomDocClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (GomDocPrivate));

    oclass->get_property = gom_doc_get_property;
    oclass->set_property = gom_doc_set_property;

    g_object_class_override_property (oclass, PROP_NODE_NAME,        "node-name");
    g_object_class_override_property (oclass, PROP_NODE_VALUE,       "node-value");
    g_object_class_override_property (oclass, PROP_NODE_TYPE,        "node-type");
    g_object_class_override_property (oclass, PROP_PARENT_NODE,      "parent-node");
    g_object_class_override_property (oclass, PROP_CHILD_NODES,      "child-nodes");
    g_object_class_override_property (oclass, PROP_FIRST_CHILD,      "first-child");
    g_object_class_override_property (oclass, PROP_LAST_CHILD,       "last-child");
    g_object_class_override_property (oclass, PROP_PREVIOUS_SIBLING, "previous-sibling");
    g_object_class_override_property (oclass, PROP_NEXT_SIBLING,     "next-sibling");
    g_object_class_override_property (oclass, PROP_ATTRIBUTES,       "attributes");
    g_object_class_override_property (oclass, PROP_OWNER_DOCUMENT,   "owner-document");

    g_object_class_override_property (oclass, PROP_DOCTYPE,          "doctype");
    g_object_class_override_property (oclass, PROP_IMPLEMENTATION,   "implementation");
    g_object_class_override_property (oclass, PROP_DOCUMENT_ELEMENT, "document-element");
}

typedef struct {
    const char *filename;
    JSContext *cx;
    JSObject *jsobj;
    GomDocument *doc;
    GSList *elems;
    GString *script;
} GomDomParserData;

static void
gom_dom_parser_start_script (GMarkupParseContext *context,
                             const gchar         *element_name,
                             const gchar        **attribute_names,
                             const gchar        **attribute_values,
                             gpointer             user_data,
                             GError             **error)
{
    GomDomParserData *data = user_data;
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
        
        if (!JS_EvaluateScript(data->cx, data->jsobj,
                               script, script_len,
                               *value, 0, &rval)) {
            if (!gom_js_exception_get_error (data->cx, error)) {
                g_set_error (error, GOM_DOC_ERROR, GOM_DOC_ERROR_UNKNOWN,
                             "Unknown error encountered while running script at %s\n",
                             *value);
            }
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
gom_dom_parser_start_element (GMarkupParseContext *context,
                              const gchar         *element_name,
                              const gchar        **attribute_names,
                              const gchar        **attribute_values,
                              gpointer             user_data,
                              GError             **error)
{
    JSObject *jsobj = NULL;
    GomElement *elem;
    GomDomParserData *data = user_data;
    GParamSpec *spec;
    const char **name, **value;
    guint signal_id;
    JSFunction *fun;
    int lineno;

    if (!strcmp (element_name, "gom") ||
        !strcmp (element_name, "config") ||
        !strcmp (element_name, "module") ||
        !strcmp (element_name, "app")) {
        return;
    }

    if (!strcmp (element_name, "script")) {
        gom_dom_parser_start_script (context, element_name, attribute_names, attribute_values, user_data, error);
        return;
    }

    elem = gom_document_create_element (data->doc, element_name, error);
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
                        g_set_error (error, GOM_DOC_ERROR, GOM_DOC_ERROR_UNKNOWN,
                                     "Could not get JSObject for elem %s",
                                     element_name);
                    }
                    return;
                }
            }
            
            g_markup_parse_context_get_position (context, &lineno, NULL);
            fun = JS_CompileFunction (data->cx, data->jsobj, NULL, 0, NULL,
                                      *value, strlen (*value),
                                      data->filename, lineno);
            
            if (!fun) {
                if (!gom_js_exception_get_error (data->cx, error)) {
                    g_set_error (error, GOM_DOC_ERROR, GOM_DOC_ERROR_UNKNOWN,
                                 "Could not compile function at %s:%d", data->filename, lineno);
                }
                return;
            }
            
            if (!gom_js_object_connect (data->cx, jsobj,
                                        signal_id,
                                        fun)) {
                if (!gom_js_exception_get_error (data->cx, error)) {
                    g_set_error (error, GOM_DOC_ERROR, GOM_DOC_ERROR_UNKNOWN,
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
    data->elems = g_slist_prepend (data->elems, elem);
}

/* Called for close tags </foo> */
static void
gom_dom_parser_end_element (GMarkupParseContext *context,
                            const gchar         *element_name,
                            gpointer             user_data,
                            GError             **error)
{
    GomDomParserData *data = user_data;

    if (!strcmp (element_name, "gom") ||
        !strcmp (element_name, "config") ||
        !strcmp (element_name, "module") ||
        !strcmp (element_name, "app")) {
        return;
    }

    if (!strcmp (element_name, "script")) {
        jsval rval; 
        JSString *str; 
        int lineno;
        /* src tag */
        if (!data->script) {
            return;
        }
        g_markup_parse_context_get_position (context, &lineno, NULL);
        if (!JS_EvaluateScript(data->cx, data->jsobj,
                               data->script->str, data->script->len,
                               data->filename, lineno, &rval)) {
            if (!gom_js_exception_get_error (data->cx, error)) {
                g_set_error (error, GOM_DOC_ERROR, GOM_DOC_ERROR_UNKNOWN,
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
    }
}

static GMarkupParser gom_dom_parser = {
    gom_dom_parser_start_element,
    gom_dom_parser_end_element,
    gom_dom_parser_text,
    NULL, NULL
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
