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

#include "gom/gomelem.h"

#include "gom/dom/gomelement.h"

#include "gommacros.h"

enum {
    PROP_TAG_NAME = 1
};

static void
gom_elem_get_property (GObject    *object,
                       guint       property_id,
                       GValue     *value,
                       GParamSpec *pspec)
{
    switch (property_id) {
    case PROP_TAG_NAME:
        g_object_get_property (object, "node-name", value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static char *
gom_elem_get_attribute_ns (GomElement *elem, const char *namespace_uri, const char *name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
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
    GOM_NOT_IMPLEMENTED;
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
    GOM_NOT_IMPLEMENTED;
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
    GOM_NOT_IMPLEMENTED;
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
    GOM_NOT_IMPLEMENTED;
    return old_attr;
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
gom_elem_class_init (GomElemClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    oclass->get_property = gom_elem_get_property;

    g_object_class_override_property (oclass, PROP_TAG_NAME, "tag-name");
}
