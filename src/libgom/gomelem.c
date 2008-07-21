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

#include "gom/dom/gomelement.h"

#include "gom/gomelem.h"

#include "gom/dom/gomdomexception.h"
#include "gom/gomobject.h"

#include "gommacros.h"

#include <string.h>

enum {
    PROP_TAG_NAME = 1,
    PROP_NODE_TYPE,
    PROP_PREFIX,
    PROP_NAMESPACE_URI,
    PROP_LOCAL_NAME
};

typedef struct {
    char *tag_name;
    char *prefix;
    char *local_name;
    char *namespace_uri;
    guint constructed : 1;
} GomElemPrivate;

#define PRIV(i) G_TYPE_INSTANCE_GET_PRIVATE ((i), GOM_TYPE_ELEM, GomElemPrivate)

static void
gom_elem_get_property (GObject    *object,
                       guint       property_id,
                       GValue     *value,
                       GParamSpec *pspec)
{
    GomElemPrivate *priv = PRIV (object);
    switch (property_id) {
    case PROP_TAG_NAME:
        g_value_set_string (value, priv->tag_name);
        break;
    case PROP_NODE_TYPE:
        g_value_set_enum (value, GOM_ELEMENT_NODE);
        break;
    case PROP_PREFIX:
        g_value_set_string (value, priv->prefix);
        break;
    case PROP_NAMESPACE_URI:
        g_value_set_string (value, priv->namespace_uri);
        break;
    case PROP_LOCAL_NAME:
        g_value_set_string (value, priv->local_name);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
update_tag_name (GObject *object)
{
    GomElemPrivate *priv = PRIV (object);
    if (priv->namespace_uri) {
        if (priv->prefix && !strcmp (priv->prefix, "xml") &&
            strcmp (priv->namespace_uri, "http://www.w3.org/XML/1998/namespace")) {
            g_warning (G_STRLOC": invalid prefix '%s' for namespace '%s' on %p",
                       priv->prefix, priv->namespace_uri, object);
            g_free (priv->prefix);
            priv->prefix = NULL;
            return;
        }
        if (!priv->local_name) {
            g_warning (G_STRLOC": no local name set on %p", object);
            return;
        }
    } else if (priv->prefix) {
        g_warning (G_STRLOC": prefix set without namespace on %p", object);
        return;
    } else if (priv->local_name) {
        g_warning (G_STRLOC": localName set without namespace on %p", object);
        return;
    } else {
        return;
    }
    g_free (priv->tag_name);
    priv->tag_name = priv->prefix ? g_strconcat (priv->prefix, ":", priv->local_name, NULL) : g_strdup (priv->local_name);
}

static void
gom_elem_set_property (GObject      *object,
                       guint         property_id,
                       const GValue *value,
                       GParamSpec   *pspec)
{
    GomElemPrivate *priv = PRIV (object);
    switch (property_id) {
    case PROP_TAG_NAME:
        priv->tag_name = g_value_dup_string (value);
        break;
    case PROP_PREFIX:
        g_free (priv->prefix);
        priv->prefix = g_value_dup_string (value);
        if (priv->constructed) {
            update_tag_name (object);
        }
        break;
    case PROP_NAMESPACE_URI:
        priv->namespace_uri = g_value_dup_string (value);
        break;
    case PROP_LOCAL_NAME:
        priv->local_name = g_value_dup_string (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static char *
gom_elem_get_attribute_ns (GomElement *elem, const char *namespace_uri, const char *name)
{
    const GValue *gvalp;
    char *ret = NULL;
    gvalp = gom_object_get_attribute (G_OBJECT (elem), name);
    if (gvalp) {
        if (G_VALUE_HOLDS_STRING (gvalp)) {
            ret = g_value_dup_string (gvalp);
        }
    }
    return ret;

}

static char *
gom_elem_get_attribute (GomElement *elem, const char *name)
{
    return gom_elem_get_attribute_ns (elem, NULL, name);
}

static void
gom_elem_set_attribute_ns (GomElement *elem,
                           const char *namespace_uri,
                           const char *qualified_name,
                           const char *value,
                           GError **error)
{
    GValue gval = { 0 };
    g_value_init (&gval, G_TYPE_STRING);
    g_value_set_string (&gval, value);
    gom_object_set_attribute (G_OBJECT (elem), qualified_name, &gval);
    g_value_unset (&gval);
}

static void
gom_elem_set_attribute (GomElement *elem, const char *name, const char *value, GError **error)
{
    gom_elem_set_attribute_ns (elem, NULL, name, value, error);
}

static void
gom_elem_remove_attribute_ns (GomElement *elem,
                                const char *namespace_uri,
                                const char *local_name,
                                GError    **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
}

static void
gom_elem_remove_attribute (GomElement *elem, const char *name, GError **error)
{
    gom_elem_remove_attribute_ns (elem, NULL, name, error);
}

static GomAttr *
gom_elem_get_attribute_node_ns (GomElement *elem,
                              const char *namespace_uri,
                              const char *local_name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomAttr *
gom_elem_get_attribute_node (GomElement *elem, const char *name)
{
    return gom_elem_get_attribute_node_ns (elem, NULL, name);
}

static GomAttr *
gom_elem_set_attribute_node_ns (GomElement *elem, GomAttr *new_atr, GError **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return new_atr;
}

static GomAttr *
gom_elem_set_attribute_node (GomElement *elem, GomAttr *new_atr, GError **error)
{
    return gom_elem_set_attribute_node_ns (elem, new_atr, error);
}

static GomAttr *
gom_elem_remove_attribute_node (GomElement *elem, GomAttr *old_attr, GError **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomNodeList *
gom_elem_get_elements_by_tag_name_ns (GomElement *elem, const char *namespace_uri, const char *name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomNodeList *
gom_elem_get_elements_by_tag_name (GomElement *elem, const char *name)
{
    return gom_elem_get_elements_by_tag_name_ns (elem, NULL, name);
}

static gboolean
gom_elem_has_attribute_ns (GomElement *elem, const char *namespace_uri, const char *local_name)
{
    GOM_NOT_IMPLEMENTED;
    return FALSE;
}    

static gboolean
gom_elem_has_attribute (GomElement *elem, const char *name)
{
    return gom_elem_has_attribute_ns (elem, NULL, name);
}

GOM_IMPLEMENT (ELEMENT, element, gom_elem);

G_DEFINE_TYPE_WITH_CODE (GomElem, gom_elem, GOM_TYPE_NOODLE,
                         GOM_IMPLEMENT_INTERFACE (ELEMENT, element, gom_elem));

static void gom_elem_init (GomElem *elem) { }

static void
gom_elem_dispose (GObject *object)
{
    GomElemPrivate *priv = PRIV (object);

    g_free (priv->namespace_uri);
    priv->namespace_uri = NULL;

    g_free (priv->prefix);
    priv->prefix = NULL;

    g_free (priv->local_name);
    priv->local_name = NULL;

    if (priv->tag_name) {
        g_print (G_STRLOC": <%s>\n", priv->tag_name);
    }
    g_free (priv->tag_name);
    priv->tag_name = NULL;

    G_OBJECT_CLASS (gom_elem_parent_class)->dispose (object);
}

static void
gom_elem_constructed (GObject *object)
{
    GomElemPrivate *priv = PRIV (object);

    G_OBJECT_CLASS (gom_elem_parent_class)->constructed (object);

    update_tag_name (object);

    priv->constructed = 1;
}

static void
gom_elem_class_init (GomElemClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (GomElemPrivate));

    oclass->get_property = gom_elem_get_property;
    oclass->set_property = gom_elem_set_property;
    oclass->constructed  = gom_elem_constructed;
    oclass->dispose      = gom_elem_dispose;

    g_object_class_override_property (oclass, PROP_TAG_NAME,      "tag-name");
    g_object_class_override_property (oclass, PROP_TAG_NAME,      "node-name");
    g_object_class_override_property (oclass, PROP_NODE_TYPE,     "node-type");
    g_object_class_override_property (oclass, PROP_PREFIX,        "prefix");
    g_object_class_override_property (oclass, PROP_NAMESPACE_URI, "namespace-u-r-i");
    g_object_class_override_property (oclass, PROP_LOCAL_NAME,    "local-name");
}
