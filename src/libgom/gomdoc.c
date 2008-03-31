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

#include "gom/gomdoc.h"

#include "gom/dom/gomdocumenttype.h"
#include "gom/dom/gomdomexception.h"
#include "gom/gomelem.h"
#include "gom/gomobject.h"

#include "gommacros.h"

#include <gtk/gtkwidget.h>

#include <string.h>

enum {
    PROP_DOCTYPE = 1,
    PROP_IMPLEMENTATION,
    PROP_DOCUMENT_ELEMENT
};

typedef struct {
    GomDOMImplementation *implementation;
    GomDocumentType      *doctype;
    guint                 constructed : 1;
} GomDocPrivate;

#define PRIV(o) G_TYPE_INSTANCE_GET_PRIVATE ((o), GOM_TYPE_DOC, GomDocPrivate)

static void
gom_doc_get_property (GObject    *object,
                      guint       property_id,
                      GValue     *value,
                      GParamSpec *pspec)
{
    GomDocPrivate *priv = PRIV (object);
    GomElement *elem;
    switch (property_id) {
    case PROP_DOCTYPE:
        g_value_set_object (value, priv->doctype);
        break;
    case PROP_IMPLEMENTATION:
        g_value_set_object (value, priv->implementation);
        break;
    case PROP_DOCUMENT_ELEMENT:
        g_object_get (object, "first-child", &elem, NULL);
        if (GOM_IS_ELEMENT (elem)) {
            g_value_set_object (value, elem);
        }
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
        }
    }
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
}

static GomElement *
gom_doc_create_element_ns (GomDocument *doc,
                           const char  *namespace_uri,
                           const char  *qualified_name,
                           GError     **error)
{
    GType type;
    GObject *obj = NULL;

    type = g_type_from_name (qualified_name);
    if (!type || !g_type_is_a (type, GOM_TYPE_ELEMENT)) {
        if (!strcmp (qualified_name, "gom") ||
            !strcmp (qualified_name, "config") ||
            !strcmp (qualified_name, "module") ||
            !strcmp (qualified_name, "app")) {
            type = GOM_TYPE_ELEM;
        } else {
            type = 0;
        }
    }
    if (type) {
        obj = g_object_new (type, 
                            "owner-document",  doc,
                            "namespace-u-r-i", namespace_uri,
                            "node-name",       qualified_name,
                            NULL);
    }
    if (!obj) {
        g_set_error (error,
                     GOM_DOM_EXCEPTION_ERROR,
                     GOM_UNKNOWN_TAG_NAME,
                     "%s is not a valid tag or registered GType",
                     qualified_name);
        return NULL;
    }

    if (GTK_IS_WIDGET (obj)) {
        gtk_widget_show (GTK_WIDGET (obj));
    }

    return GOM_ELEMENT (obj);
}

static GomElement *
gom_doc_create_element (GomDocument *doc,
                        const char  *tag_name,
                        GError     **error)
{
    return gom_doc_create_element_ns (doc, NULL, tag_name, error);
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
gom_doc_create_attribute_ns (GomDocument *doc,
                             const char  *namespace_uri,
                             const char  *qualified_name,
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
    return gom_doc_create_attribute_ns (doc, NULL, name, error);
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
gom_doc_get_elements_by_tag_name_ns (GomDocument *doc,
                                     const char  *namespace_uri,
                                     const char  *qualified_name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
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
                           const char  *element_id)
 {
    GomNodeList *children;
    GomNode *node;
    GomElement *elem = NULL;
    gulong i, length;
    
    g_object_get (doc, "child-nodes", &children, NULL);
    g_object_get (children, "length", &length, NULL);
    for (i = 0; i < length; i++) {
        node = gom_node_list_item (children, i);
        if (GOM_IS_ELEMENT (node)) {
            elem = element_get_element_by_id (GOM_ELEMENT (node), element_id);
            if (elem) {
                break;
            }
        }
    }
    g_object_unref (children);
    return elem;
}

GOM_IMPLEMENT (DOCUMENT, document, gom_doc);

G_DEFINE_TYPE_WITH_CODE (GomDoc, gom_doc, GOM_TYPE_NOODLE,
                         GOM_IMPLEMENT_INTERFACE (DOCUMENT, document, gom_doc));

static void gom_doc_init (GomDoc *doc) { }

static GObject *
gom_doc_constructor (GType                  type, 
                     guint                  n_construct_properties,
                     GObjectConstructParam *construct_properties)
{
    GObject *object;

    object = G_OBJECT_CLASS (gom_doc_parent_class)->constructor (
        type, n_construct_properties, construct_properties);

    g_object_set (object,
                  "owner-document",  object,
                  "node-name",       "document",
                  "node-type",       GOM_DOCUMENT_NODE,
                  NULL);

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
gom_doc_finalize (GObject *object)
{
    GomDocPrivate *priv = PRIV (object);
    g_print ("%s:%d:%s (%s %p)\n",
             __FILE__, __LINE__, __FUNCTION__,
             g_type_name (G_TYPE_FROM_INSTANCE (object)), object);
    if (priv->doctype) {
        g_object_unref (priv->doctype);
        priv->doctype = NULL;
    }
    if (priv->implementation) {
        g_object_unref (priv->implementation);
        priv->implementation = NULL;
    }
    G_OBJECT_CLASS (gom_doc_parent_class)->finalize (object);
}

static void
gom_doc_class_init (GomDocClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (GomDocPrivate));

    oclass->constructor  = gom_doc_constructor;
    oclass->constructed  = gom_doc_constructed;
    oclass->finalize     = gom_doc_finalize;
    oclass->get_property = gom_doc_get_property;
    oclass->set_property = gom_doc_set_property;

    g_object_class_override_property (oclass, PROP_DOCTYPE,          "doctype");
    g_object_class_override_property (oclass, PROP_IMPLEMENTATION,   "implementation");
    g_object_class_override_property (oclass, PROP_DOCUMENT_ELEMENT, "document-element");
}
