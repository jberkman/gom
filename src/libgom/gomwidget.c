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

#include <gom/gomwidget.h>

#include <gommacros.h>

#include <gom/dom/gomdomexception.h>
#include <gom/dom/gomelement.h>
#include <gom/gomglist.h>
#include <gom/gomjselement.h>
#include <gom/gomjsobject.h>
#include <gom/gomobject.h>

#include <gtk/gtk.h>

#include <strings.h>
#include <stdlib.h>
#include <limits.h>

enum {
    PROP_NODE_NAME = 1078,
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

    PROP_TAG_NAME
};

static void (*gtk_widget_set_property) (GObject        *object,
                                        guint           property_id,
                                        const GValue   *value,
                                        GParamSpec     *pspec);
static void (*gtk_widget_get_property) (GObject        *object,
                                        guint           property_id,
                                        GValue         *value,
                                        GParamSpec     *pspec);

static GtkBuilder *builder = NULL;

/* attributes */

static GomNode *
widget_get_first_child (GomNode *node)
{
    if (GTK_IS_CONTAINER (node)) {
        GList *children = gtk_container_get_children (GTK_CONTAINER (node));
        GomNode *child = children ? children->data : NULL;
        g_list_free (children);
        return child;
    }
    return NULL;
}

static GomNode *
widget_get_last_child (GomNode *node)
{
    if (GTK_IS_CONTAINER (node)) {
        GList *children = gtk_container_get_children (GTK_CONTAINER (node));
        GList *last = g_list_last (children);
        GomNode *child = last ? last->data : NULL;
        g_list_free (children);
        return child;
    }
    return NULL;
}

static GomNode *
widget_get_previous_sibling (GomNode *node)
{
    GomNode *sib = NULL;
    GtkWidget *parent = gtk_widget_get_parent (GTK_WIDGET (node));
    if (parent && GTK_IS_CONTAINER (parent)) {
        GList *children = gtk_container_get_children (GTK_CONTAINER (parent));
        GList *self = g_list_find (children, GTK_WIDGET (node));
        if (self) {
            sib = self && self->prev ? GOM_NODE (self->prev->data) : NULL;
        }
        g_list_free (children);
    }
    return sib;
}

static GomNode *
widget_get_next_sibling (GomNode *node)
{
    GomNode *sib = NULL;
    GtkWidget *parent = gtk_widget_get_parent (GTK_WIDGET (node));
    if (parent && GTK_IS_CONTAINER (parent)) {
        GList *children = gtk_container_get_children (GTK_CONTAINER (parent));
        GList *self = g_list_find (children, GTK_WIDGET (node));
        if (self) {
            sib = self && self->next ? GOM_NODE (self->next->data) : NULL;
        }
        g_list_free (children);
    }
    return sib;
}

static GomNamedNodeMap *
widget_get_attributes (GomNode *node)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomDocument *
widget_get_owner_document (GomNode *node)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomNode *
widget_insert_before (GomNode *node,
                      GomNode *new_child,
                      GomNode *ref_child,
                      GError  **error)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomNode *
widget_replace_child (GomNode *node,
                      GomNode *new_child,
                      GomNode *ref_child,
                      GError  **error)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomNode *
widget_remove_child (GomNode *node,
                     GomNode *old_child,
                     GError  **error)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

typedef struct {
    GomNode *node;
    GomNode *new_child;
    GError **error;
} AppendChildData;

static void
append_child_attrs_foreach (gpointer key, gpointer value, gpointer user_data)
{
    AppendChildData *data = user_data;
    GParamSpec *spec;
    GError *error = NULL;
    GValue gval = { 0 };

    spec = gtk_container_class_find_child_property (G_OBJECT_GET_CLASS (data->node), key);
    if (!spec) {
        return;
    }
    g_print ("found child property %s.%s on %s\n",
             g_type_name (G_TYPE_FROM_INSTANCE (data->node)),
             spec->name,
             g_type_name (G_TYPE_FROM_INSTANCE (data->new_child)));
    
    if (gtk_builder_value_from_string (builder, spec, g_value_get_string (value), &gval, &error)) {
        gtk_container_child_set_property (GTK_CONTAINER (data->node),
                                          GTK_WIDGET (data->new_child),
                                          spec->name, 
                                          &gval);
        g_value_unset (&gval);
    } else {
        g_print ("Error getting value_from_string: %s\n",
                 error->message);
        if (data->error && *data->error) {
            g_error_free (error);
        } else {
            g_propagate_error (data->error, error);
        }
    }
}

static GomNode *
widget_append_child (GomNode *node,
                     GomNode *new_child,
                     GError  **error)
{
    AppendChildData data;

    if (!GTK_IS_CONTAINER (node)) {
        g_set_error (error,
                     GOM_DOM_EXCEPTION_ERROR,
                     GOM_DOM_EXCEPTION_ERROR_HIERCHY_REQUEST_ERR,
                     "Cannot append a child that is not a GtkWidget");
        return NULL;
    }

    if (!GTK_IS_WIDGET (new_child)) {
        g_set_error (error,
                     GOM_DOM_EXCEPTION_ERROR,
                     GOM_DOM_EXCEPTION_ERROR_HIERCHY_REQUEST_ERR,
                     "Cannot append to a parent that is not a GtkContainer");
        return NULL;
    }

    gtk_container_add (GTK_CONTAINER (node), GTK_WIDGET (new_child));

    data.node = node;
    data.new_child = new_child;
    data.error = error;

    gom_object_attributes_foreach (G_OBJECT (new_child), append_child_attrs_foreach, &data);

    return error && *error ? NULL : new_child;
}

static gboolean
widget_has_child_nodes (GomNode *node)
{
    gboolean has_child_nodes = FALSE;

    if (GTK_IS_CONTAINER (node)) {
        GList *children = gtk_container_get_children (GTK_CONTAINER (node));
        has_child_nodes = g_list_length (children) > 0;
        g_list_free (children);
    }
    return has_child_nodes;
}

static GomNode *
widget_clone_node (GomNode *node, gboolean deep)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static void
widget_node_init (gpointer g_iface, gpointer iface_data)
{
    GomNodeInterface *node = (GomNodeInterface *)g_iface;

    if (!builder) {
        builder = gtk_builder_new ();
    }

#define IFACE(func) node->func = widget_##func

    IFACE (insert_before);
    IFACE (replace_child);
    IFACE (remove_child);
    IFACE (append_child);
    IFACE (has_child_nodes);
    IFACE (clone_node);

#undef IFACE
}

/* GomElement implementation */

static char *
widget_get_attribute (GomElement *elem, const char *name)
{
    GParamSpec *spec;
    GValue gval = { 0 };
    GValue *gvalp;
    const char *ret = NULL;

    spec = g_object_class_find_property (G_OBJECT_GET_CLASS (elem), name);
    if (spec) {
        g_value_init (&gval, G_TYPE_STRING);
        g_object_get_property (G_OBJECT (elem), name, &gval);
        if (G_VALUE_HOLDS (&gval, G_TYPE_STRING)) {
            ret = g_value_get_string (&gval);
        }
    } else {
        gvalp = gom_object_get_attribute (G_OBJECT (elem), name);
        if (gvalp && G_VALUE_HOLDS_STRING (gvalp)) {
            ret = g_value_get_string (gvalp);
        }
    }

    return g_strdup (ret);
}

static void
widget_set_attribute (GomElement *elem, const char *name, const char *value, GError **error)
{
    GParamSpec *spec;
    GValue gval = { 0 };

    spec = g_object_class_find_property (G_OBJECT_GET_CLASS (elem), name);
    if (spec) {
        if (gtk_builder_value_from_string (builder, spec, value, &gval, NULL)) {
            g_object_set_property (G_OBJECT (elem), name, &gval);
            g_value_unset (&gval);
        }
    } else {
        g_value_init (&gval, G_TYPE_STRING);
        g_value_set_string (&gval, value);
        gom_object_set_attribute (G_OBJECT (elem), name, &gval);
        g_value_unset (&gval);
    }
}

static void
widget_remove_attribute (GomElement *elem, const char *name, GError **error)
{
    GOM_NOT_IMPLEMENTED;
}

static GomAttr *
widget_get_attribute_node (GomElement *elem, const char *name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static void
widget_set_attribute_node (GomElement *elem, GomAttr *new_atr, GError **error)
{
    GOM_NOT_IMPLEMENTED;
}

static void
widget_remove_attribute_node (GomElement *elem, GomAttr *old_attr, GError **error)
{
    GOM_NOT_IMPLEMENTED;
}

static GomNodeList *
widget_get_elements_by_tag_name (GomElement *elem, const char *name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static void
widget_normalize (GomElement *elem)
{
    GOM_NOT_IMPLEMENTED;
}

static void
gom_widget_get_property (GObject    *object,
                         guint       property_id,
                         GValue     *value,
                         GParamSpec *pspec)
{
    switch (property_id) {
    case PROP_TAG_NAME:
    case PROP_NODE_NAME:
        g_value_set_static_string (value, g_type_name (G_TYPE_FROM_INSTANCE (object)));
        break;
    case PROP_NODE_VALUE:
        GOM_NOT_IMPLEMENTED;
        break;
    case PROP_NODE_TYPE:
        g_value_set_uint (value, GOM_ELEMENT_NODE);
        break;
    case PROP_PARENT_NODE:
        g_value_set_object (value, gtk_widget_get_parent (GTK_WIDGET (object)));
        break;
    case PROP_CHILD_NODES:
        g_value_set_object (value, 
                            GTK_IS_CONTAINER (object)
                            ? gom_g_list_new (gtk_container_get_children (GTK_CONTAINER (object)))
                            : NULL);
        break;
    case PROP_FIRST_CHILD:
        g_value_set_object (value, widget_get_first_child (GOM_NODE (object)));
        break;
    case PROP_LAST_CHILD:
        g_value_set_object (value, widget_get_last_child (GOM_NODE (object)));
        break;
    case PROP_PREVIOUS_SIBLING:
        g_value_set_object (value, widget_get_previous_sibling (GOM_NODE (object)));
        break;
    case PROP_NEXT_SIBLING:
        g_value_set_object (value, widget_get_next_sibling (GOM_NODE (object)));
        break;
    case PROP_ATTRIBUTES:
        g_value_set_object (value, widget_get_attributes (GOM_NODE (object)));
        break;
    case PROP_OWNER_DOCUMENT:
        g_value_set_object (value, widget_get_owner_document (GOM_NODE (object)));
        break;
    default:
        gtk_widget_get_property (object, property_id, value, pspec);
        break;
    }
}

static void
gom_widget_set_property (GObject      *object,
                         guint         property_id,
                         const GValue *value,
                         GParamSpec   *pspec)
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
    case PROP_TAG_NAME:
        g_assert_not_reached ();
        break;
    default:
        gtk_widget_set_property (object, property_id, value, pspec);
        break;
    }
}

static void
widget_element_init (gpointer g_iface, gpointer iface_data)
{
    GomElementInterface *elem = (GomElementInterface *)g_iface;

#define IFACE(func) elem->func = widget_##func

    IFACE (get_attribute);
    IFACE (set_attribute);
    IFACE (remove_attribute);
    IFACE (get_attribute_node);
    IFACE (set_attribute_node);
    IFACE (remove_attribute_node);
    IFACE (get_elements_by_tag_name);
    IFACE (normalize);

#undef IFACE
}

void
gom_widget_init_log_func (const gchar   *log_domain,
                          GLogLevelFlags log_level,
                          const gchar   *message,
                          gpointer       user_data)
{
    if (log_level == G_LOG_LEVEL_CRITICAL &&
        !strcmp (log_domain, "GLib-GObject") &&
        g_str_has_prefix (message, "Object class GtkWidget doesn't implement property '") &&
        (g_str_has_suffix (message, "' from interface 'GomNode'") ||
         g_str_has_suffix (message, "' from interface 'GomElement'"))) {
        return;
    }
    g_log_default_handler (log_domain, log_level, message, user_data);
}

static gpointer
gom_widget_init_once (gpointer data)
{
    GObjectClass *oclass;
    GType type = 0;
    static const GInterfaceInfo node_info = {
        widget_node_init,
        NULL, /* interface_finalize */
        NULL  /* interface_data */
    };
    static const GInterfaceInfo element_info = {
        widget_element_init,
        NULL, /* interface_finalize */
        NULL  /* interface_data */
    };

    g_type_add_interface_static (GTK_TYPE_WIDGET, GOM_TYPE_NODE, &node_info);
    g_type_add_interface_static (GTK_TYPE_WIDGET, GOM_TYPE_ELEMENT, &element_info);

    oclass = g_type_class_ref (GTK_TYPE_WIDGET);

    gtk_widget_get_property = oclass->get_property;
    oclass->get_property = gom_widget_get_property;

    gtk_widget_set_property = oclass->set_property;
    oclass->set_property = gom_widget_set_property;

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

    g_object_class_override_property (oclass, PROP_TAG_NAME,         "tag-name");

#define WIDGET(w) type ^= (w);
#include "gomwidgets.c"
#undef WIDGET

    return GSIZE_TO_POINTER (type);
}

void
gom_widget_init (void)
{
    static GOnce init_once = G_ONCE_INIT;
    g_once (&init_once, gom_widget_init_once, NULL);
}
