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

#include "gom/dom/gomdocument.h"

#include "gom/gomdoc.h"

#include "gom/dom/gomdocumentevent.h"
#include "gom/dom/gomdocumenttype.h"
#include "gom/dom/gomevent.h"
#include "gom/dom/gomdomexception.h"
#include "gom/dom/gomkeyboardevent.h"
#include "gom/dom/gommouseevent.h"
#include "gom/gomcdata.h"
#include "gom/gomcmt.h"
#include "gom/gomelem.h"
#include "gom/gomglist.h"
#include "gom/gomkeyboardevt.h"
#include "gom/gommouseevt.h"
#include "gom/gomobject.h"
#include "gom/gomtxt.h"
#include "gom/gomunknown.h"

#include "gommacros.h"

#include <gtk/gtkmain.h>
#include <gtk/gtkwidget.h>
#include <gtk/gtkwindow.h>

#include <string.h>

enum {
    PROP_DOCTYPE = 1,
    PROP_IMPLEMENTATION,
    PROP_DOCUMENT_ELEMENT,
    PROP_NODE_NAME,
    PROP_NODE_TYPE,
    PROP_DOCUMENT_URI
};

typedef struct {
    GomDOMImplementation *implementation;
    GomDocumentType      *doctype;
    guint                 constructed : 1;
    const char           *document_uri;
} GomDocPrivate;

#define PRIV(o) G_TYPE_INSTANCE_GET_PRIVATE ((o), GOM_TYPE_DOC, GomDocPrivate)

static void
gom_doc_get_property (GObject    *object,
                      guint       property_id,
                      GValue     *value,
                      GParamSpec *pspec)
{
    GomDocPrivate *priv = PRIV (object);
    gpointer elemp;
    GomElement *elem;
    switch (property_id) {
    case PROP_DOCTYPE:
        g_value_set_object (value, priv->doctype);
        break;
    case PROP_IMPLEMENTATION:
        g_value_set_object (value, priv->implementation);
        break;
    case PROP_DOCUMENT_ELEMENT:
        g_object_get (object, "first-child", &elemp, NULL);
        if (G_IS_OBJECT (elemp)) {
            elem = gom_unknown_query_interface (elemp, GOM_TYPE_ELEMENT, NULL);
            if (elem) {
                g_value_set_object (value, elem);
                g_object_unref (elem);
            }
        }
        if (G_IS_OBJECT (elemp)) {
            g_object_unref (elemp);
        }
        break;
    case PROP_NODE_NAME:
        g_value_set_static_string (value, "#document");
        break;
    case PROP_NODE_TYPE:
        g_value_set_enum (value, GOM_DOCUMENT_NODE);
        break;
    case PROP_DOCUMENT_URI:
        g_value_set_string (value, priv->document_uri);
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
    GomDocPrivate *priv = PRIV (object);
    if (!priv->constructed) {
        switch (property_id) {
        case PROP_DOCTYPE:
            priv->doctype = g_value_dup_object (value);
            return;
        case PROP_IMPLEMENTATION:
            priv->implementation = g_value_dup_object (value);
            return;
        case PROP_DOCUMENT_URI:
            priv->document_uri = g_value_dup_string (value);
            return;
        }
    }
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
}

static gboolean
delete_window (GtkWidget *w)
{
    GomEvent         *unload;
    GomDocumentEvent *doc;
    GError           *err = NULL;

    g_object_get (w, "owner-document", &doc, NULL);
    g_assert (GOM_IS_DOCUMENT_EVENT (doc));

    unload = gom_document_event_create_event (doc, "Event", &err);
    if (err) {
        g_error (GOM_LOC ("Could not create an unload event: %s"), err->message);
        g_assert_not_reached ();
        return TRUE;
    }

    gom_event_init_event (unload, "unload", FALSE, FALSE);
    gom_event_target_dispatch_event (GOM_EVENT_TARGET (doc), unload, &err);
    if (err) {
        g_error (GOM_LOC ("Error dispatching unload event: %s"), err->message);
        g_assert_not_reached ();
        return TRUE;
    }

    g_object_unref (unload);
    g_object_unref (doc);

    gtk_main_quit ();

    return TRUE;
}

static GomElement *
gom_doc_create_element_ns (GomDocument *doc,
                           const char  *namespace_uri,
                           const char  *qualified_name,
                           GError     **error)
{
    GType type;
    GObject *obj = NULL;
    char *prefix = NULL;
    const char *local_name;
    char *colon;

    colon = strchr (qualified_name, ':');
    if (colon) {
        prefix = g_strdup (qualified_name);
        colon = strchr (prefix, ':');
        *colon = '\0';
        if (!namespace_uri) {
            g_free (prefix);
            g_set_error (error,
                         GOM_DOM_EXCEPTION_ERROR,
                         GOM_NAMESPACE_ERR,
                         "%s has a prefix, but no namespace is set",
                         qualified_name);
            return NULL;
        } else if (!strcmp (prefix, "xml") &&
                   strcmp (namespace_uri, "http://www.w3.org/XML/1998/namespace")) {
            g_free (prefix);
            g_set_error (error,
                         GOM_DOM_EXCEPTION_ERROR,
                         GOM_NAMESPACE_ERR,
                         "%s has a prefix of xml, but the namespace is not \"http://www.w3.org/XML/1998/namespace\"",
                         qualified_name);
            return NULL;
        }
        local_name = colon + 1;
    } else {
        local_name = qualified_name;
    }

    type = g_type_from_name (local_name);
    if (!type || !g_type_is_a (type, GOM_TYPE_ELEMENT)) {
        type = GOM_TYPE_ELEM;
    }
    g_print (GOM_LOC ("<%s:%s xmlns='%s'> (%s)\n"),
                      prefix, local_name, namespace_uri,
                      g_type_name (type));
    obj = g_object_new (type, 
                        "owner-document",  doc,
                        "namespace-u-r-i", namespace_uri,
                        "prefix",          prefix,
                        "local-name",      local_name,
                        NULL);
    g_free (prefix);
    if (!obj) {
        g_set_error (error,
                     GOM_DOM_EXCEPTION_ERROR,
                     GOM_UNKNOWN_TAG_NAME_ERR,
                     "%s is not a valid tag or registered GType",
                     qualified_name);
        return NULL;
    }

    if (GTK_IS_WIDGET (obj)) {
        if (GTK_IS_WINDOW (obj)) {
            g_signal_connect (obj, "delete-event", G_CALLBACK (delete_window), NULL);
        }
        gtk_widget_show (GTK_WIDGET (obj));
    }

    return GOM_ELEMENT (obj);
}

/* it would be great if we could just call the above with a namespace
 * of NULL but the semantics are just a bit different */
static GomElement *
gom_doc_create_element (GomDocument *doc,
                        const char  *tag_name,
                        GError     **error)
{
    GType type;
    GObject *obj = NULL;

    type = g_type_from_name (tag_name);
    if (!type || !g_type_is_a (type, GOM_TYPE_ELEMENT)) {
        type = GOM_TYPE_ELEM;
    }
    obj = g_object_new (type, 
                        "owner-document",  doc,
                        "tag-name",        tag_name,
                        NULL);
    if (!obj) {
        g_set_error (error,
                     GOM_DOM_EXCEPTION_ERROR,
                     GOM_UNKNOWN_TAG_NAME_ERR,
                     "%s is not a valid tag or registered GType",
                     tag_name);
        return NULL;
    }

    if (GTK_IS_WIDGET (obj)) {
        if (GTK_IS_WINDOW (obj)) {
            g_signal_connect (obj, "delete-event", G_CALLBACK (delete_window), NULL);
        }
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
    return g_object_new (GOM_TYPE_TXT, 
                         "owner-document", doc,
                         "data",           data,
                         NULL);
}

static GomComment *
gom_doc_create_comment (GomDocument *doc,
                        const char  *data)
{
    return g_object_new (GOM_TYPE_CMT,
                         "owner-document", doc,
                         "data", data,
                         NULL);
}

static GomCDATASection *
gom_doc_create_cdata_section (GomDocument *doc,
                              const char  *data,
                              GError     **error)
{
    return g_object_new (GOM_TYPE_CDATA,
                         "owner-document", doc,
                         "data", data,
                         NULL);
}

static GomProcessingInstruction *
gom_doc_create_processing_instruction (GomDocument *doc,
                                       const char  *target,
                                       const char  *data,
                                       GError     **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomAttr *
gom_doc_create_attribute_ns (GomDocument *doc,
                             const char  *namespace_uri,
                             const char  *qualified_name,
                             GError     **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomAttr *
gom_doc_create_attribute (GomDocument *doc,
                          const char  *name,
                          GError     **error)
{
    return gom_doc_create_attribute_ns (doc, NULL, name, error);
}

static GomEntityReference *
gom_doc_create_entity_reference (GomDocument *doc,
                                 const char  *name,
                                 GError     **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static gboolean
matches_by_tag_name (GomNode    *node,
                     const char *namespace_uri,
                     const char *qualified_name)
{
    char *tag_prop;
    char *ns_prop;
    gboolean retval = FALSE;

    /* i have a paper here that entitles me to fast track status. */
    if (!strcmp (qualified_name, "*")) {
        return TRUE;
    }
    g_object_get (node,
                  "namespace-uri", &ns_prop,
                  "tag-name", &tag_prop,
                  NULL);
    
    retval = !strcmp (qualified_name, tag_prop) &&
        (!namespace_uri || !strcmp (namespace_uri, ns_prop));

    g_print ("<%s xmlns=\"%s\"> == <%s xmlns=\"%s\"> => %d\n",
             tag_prop, ns_prop,
             qualified_name, namespace_uri,
             retval);

    g_free (ns_prop);
    g_free (tag_prop);

    return retval;
}
                     

static GList *
get_elements_by_tag_name (GList      *li,
                          GomNode    *node,
                          const char *namespace_uri,
                          const char *qualified_name)
{
    GomElement *elem;
    GomNode *old_node;
    
    if (matches_by_tag_name (node, namespace_uri, qualified_name)) {
        li = g_list_prepend (li, g_object_ref (node));
    }

    for (g_object_get (node, "first-child", &node, NULL);
         node;
         old_node = node,
             g_object_get (old_node, "next-sibling", &node, NULL),
             g_object_unref (old_node)) {
        elem = gom_unknown_query_interface (node, GOM_TYPE_ELEMENT, NULL);
        if (elem) {
            li = get_elements_by_tag_name (li, GOM_NODE (elem), namespace_uri, qualified_name);
            g_object_unref (elem);
        }
    }

    return li;
}

static GomNodeList *
gom_doc_get_elements_by_tag_name_ns (GomDocument *doc,
                                     const char  *namespace_uri,
                                     const char  *qualified_name)
{
    GomNodeList *ret;
    GomNode     *node;
    GList       *li;

    g_object_get (doc, "document-element", &node, NULL);

    li = g_list_reverse (get_elements_by_tag_name (NULL, node, namespace_uri, qualified_name));

    g_object_unref (node);

    ret = GOM_NODE_LIST (g_object_new (GOM_TYPE_G_LIST, "nodes", li, NULL));

    g_list_foreach (li, (GFunc)g_object_unref, NULL);
    g_list_free (li);

    return ret;
}

static GomNodeList *
gom_doc_get_elements_by_tag_name (GomDocument *doc,
                                  const char  *tag_name)
{
    return gom_doc_get_elements_by_tag_name_ns (doc, NULL, tag_name);
}

static GomNode *
gom_doc_import_node (GomDocument *doc,
                     GomNode     *node,
                     gboolean     deep,
                     GError     **error)
{
    GomNodeType node_type;
    GomNode *ret = NULL;
    char *data;

    g_object_get (node, "node-type", &node_type, NULL);
    switch (node_type) {
    case GOM_ELEMENT_NODE:
        {
            char *namespace_uri;
            char *qualified_name;
            g_object_get (node,
                          "namespace-u-r-i", &namespace_uri,
                          "node-name", &qualified_name,
                          NULL);
            ret = (GomNode *)gom_document_create_element_ns (doc, namespace_uri, qualified_name, error);
            g_free (namespace_uri);
            g_free (qualified_name);
        }
        if (!ret) {
            break;
        }
        {
            GomNamedNodeMap *attrs;
            gulong i, len;
            char *name, *value;
            gboolean specified;
            GomNode *attr;
            GError *error = NULL;
            g_object_get (node, "attributes", &attrs, NULL);
            if (attrs) {
                g_object_get (attrs, "length", &len, NULL);
                for (i = 0; i < len; i++) {
                    attr = gom_named_node_map_item (attrs, i);
                    g_object_get (attr, "specified", &specified, NULL);
                    if (specified) {
                        g_object_get (attr, "name", &name, "value", &value, NULL);
                        gom_element_set_attribute (GOM_ELEMENT (ret), name, value, &error);
                        if (error) {
                            g_warning (GOM_LOC ("setting attribute <%s %s=\"%s\"> failed: %s"),
                                       G_OBJECT_TYPE_NAME (ret), name, value,
                                       error->message);
                            g_error_free (error);
                        }
                        g_free (name);
                        g_free (value);
                    }
                    g_object_unref (attr);
                }
                g_object_unref (attrs);
            }
        }
        if (!deep) {
            break;
        }
        {
            GomNode *child, *old_child, *imported_child;
            for (g_object_get (node, "first-child", &child, NULL);
                 child;
                 old_child = child,
                     g_object_get (old_child, "next-sibling", &child, NULL),
                     g_object_unref (old_child)) {
                imported_child = gom_document_import_node (doc, child, deep, error);
                if (!imported_child) {
                    g_object_unref (ret);
                    g_object_unref (child);
                    return NULL;
                }
                if (!gom_node_append_child (ret, imported_child, error)) {
                    g_object_unref (ret);
                    g_object_unref (child);
                    return NULL;
                }
            }
        }
        break;
    case GOM_TEXT_NODE:
        g_object_get (node, "node-value", &data, NULL);
        ret = (GomNode *)gom_document_create_text_node (doc, data);
        g_free (data);
        break;
    case GOM_CDATA_SECTION_NODE:
        g_object_get (node, "node-value", &data, NULL);
        ret = (GomNode *)gom_document_create_cdata_section (doc, data, error);
        g_free (data);
        break;
    case GOM_COMMENT_NODE:
        g_object_get (node, "node-value", &data, NULL);
        ret = (GomNode *)gom_document_create_comment (doc, data);
        g_free (data);
        break;
    case GOM_DOCUMENT_NODE:
    case GOM_DOCUMENT_TYPE_NODE:
        g_set_error (error, GOM_DOM_EXCEPTION_ERROR, GOM_NOT_SUPPORTED_ERR,
                     GOM_LOC ("import not supported for type: %d"), node_type);
        break;
    default:
        g_print ("Unimplemented type: %d\n", node_type);
        GOM_NOT_IMPLEMENTED_ERROR (error);
        break;
    }
    return ret;
}

static GomElement *
get_element_by_id (GomElement *node,
                   const char *element_id)
{
    const GValue *gval;
    GomElement *old_node;
    GomElement *ret;
    GomElement *elem;

    for (g_object_get (node, "first-child", &node, NULL);
         node;
         old_node = node,
             g_object_get (old_node, "next-sibling", &node, NULL),
             g_object_unref (old_node)) {
        elem = gom_unknown_query_interface (node, GOM_TYPE_ELEMENT, NULL);
        if (!elem) {
            continue;
        }
        gval = gom_object_get_attribute (G_OBJECT (elem), "id");
        if (gval && G_VALUE_HOLDS_STRING (gval) && !strcmp (g_value_get_string (gval), element_id)) {
            g_object_unref (node);
            return elem;
        }

        ret = get_element_by_id (elem, element_id);
        g_object_unref (elem);
        if (ret) {
            g_object_unref (node);
            return ret;
        }
    }

    return NULL;
}

/* Introduced in DOM Level 2: */
static GomElement *
gom_doc_get_element_by_id (GomDocument *doc,
                           const char  *element_id)
{
    GomElement *elem;
    GomElement *root;

    g_object_get (doc, "document-element", &root, NULL);
    elem = get_element_by_id (root, element_id);
    g_object_unref (root);

    return elem;
}

static GomEvent *
gom_doc_create_event (GomDocumentEvent *gom_document_event,
                      const char       *event_type,
                      GError          **error)
{
    GType type = 0;
    switch (event_type[0]) {
    case 'E':
        if (!strcmp (event_type, "Event") ||
            !strcmp (event_type, "Events")) {
            type = GOM_TYPE_EVT;
        }
        break;
    case 'M':
        if (!strcmp (event_type, "MouseEvent") ||
            !strcmp (event_type, "MouseEvents")) {
            type = GOM_TYPE_MOUSE_EVT;
#if 0
        } else if (!strcmp (event_type, "MutationEvent") ||
                   !strcmp (event_type, "MutationEvents")) {
            type = GOM_TYPE_MUTATION_EVT;
        } else if (!strcmp (event_type, "MutationNameEvent") ||
                   !strcmp (event_type, "MutationNameEvents")) {
            type = GOM_TYPE_MUTATION_NAME_EVT;
#endif
        }
        break;
    case 'K':
        if (!strcmp (event_type, "KeyboardEvent") ||
            !strcmp (event_type, "KeyboardEvents")) {
            type = GOM_TYPE_KEYBOARD_EVT;
        }
        break;
    case 'U':
        if (!strcmp (event_type, "UIEvent") ||
            !strcmp (event_type, "UIEvents")) {
            type = GOM_TYPE_UI_EVT;
        }
        break;
    }
    if (!type) {
        g_set_error (error,
                     GOM_DOM_EXCEPTION_ERROR,
                     GOM_NOT_SUPPORTED_ERR,
                     "%s is not a supported event type",
                     event_type);
        return NULL;
    } else {
        return g_object_new (type, NULL);
    }
}

static gboolean
gom_doc_can_dispatch (GomDocumentEvent *gom_document_event,
                      const char       *namespace_uri,
                      const char       *type)
{
    GOM_NOT_IMPLEMENTED;
    return FALSE;
}

GOM_IMPLEMENT (DOCUMENT,       document,       gom_doc);
GOM_IMPLEMENT (DOCUMENT_EVENT, document_event, gom_doc);

G_DEFINE_TYPE_WITH_CODE (GomDoc, gom_doc, GOM_TYPE_NOODLE,
                         {
                             GOM_IMPLEMENT_INTERFACE (DOCUMENT,       document,       gom_doc);
                             GOM_IMPLEMENT_INTERFACE (DOCUMENT_EVENT, document_event, gom_doc);
                         });

static void gom_doc_init (GomDoc *doc) { }

static GObject *
gom_doc_constructor (GType                  type, 
                     guint                  n_construct_properties,
                     GObjectConstructParam *construct_properties)
{
    GObject *object;

    object = G_OBJECT_CLASS (gom_doc_parent_class)->constructor (
        type, n_construct_properties, construct_properties);

    g_object_set (object, "owner-document", object, NULL);

    return object;
}
static void
gom_doc_constructed (GObject *object)
{
    GomDocPrivate *priv = PRIV (object);

    if (G_OBJECT_CLASS (gom_doc_parent_class)->constructed) {
        G_OBJECT_CLASS (gom_doc_parent_class)->constructed (object);
    }

    priv->constructed = TRUE;
}

static void
gom_doc_dispose (GObject *object)
{
    GomDocPrivate *priv = PRIV (object);
    g_print (GOM_LOC ("%s %p\n"),
              g_type_name (G_TYPE_FROM_INSTANCE (object)), object);
    if (priv->doctype) {
        g_object_unref (priv->doctype);
        priv->doctype = NULL;
    }
    if (priv->implementation) {
        g_object_unref (priv->implementation);
        priv->implementation = NULL;
    }
    G_OBJECT_CLASS (gom_doc_parent_class)->dispose (object);
}

static void
gom_doc_class_init (GomDocClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (GomDocPrivate));

    oclass->constructor  = gom_doc_constructor;
    oclass->constructed  = gom_doc_constructed;
    oclass->dispose      = gom_doc_dispose;
    oclass->get_property = gom_doc_get_property;
    oclass->set_property = gom_doc_set_property;

    g_object_class_override_property (oclass, PROP_DOCTYPE,          "doctype");
    g_object_class_override_property (oclass, PROP_IMPLEMENTATION,   "implementation");
    g_object_class_override_property (oclass, PROP_DOCUMENT_ELEMENT, "document-element");
    g_object_class_override_property (oclass, PROP_NODE_NAME,        "node-name");
    g_object_class_override_property (oclass, PROP_NODE_TYPE,        "node-type");
    g_object_class_override_property (oclass, PROP_DOCUMENT_URI,     "document-u-r-i");
}
