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

static GtkBuilder *builder = NULL;

/* attributes */

static G_CONST_RETURN char *
widget_get_node_name (GomNode *node)
{
    return g_type_name (G_TYPE_FROM_INSTANCE (node));
}

static char *
widget_get_node_value (GomNode *node, GError **error)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static void
widget_set_node_value (GomNode *node, const char *value, GError **error)
{
    GOM_NOT_IMPLEMENTED;
}

static GomNodeType
widget_get_node_type (GomNode *node)
{
    return GOM_ELEMENT_NODE;
}

static GomNode *
widget_get_parent_node (GomNode *node)
{
    GtkWidget *parent = gtk_widget_get_parent (GTK_WIDGET (node));
    return parent ? GOM_NODE (parent) : NULL;
}

static GomNodeList *
widget_get_child_nodes (GomNode *node)
{
    if (GTK_IS_CONTAINER (node)) {
        return gom_g_list_new (gtk_container_get_children (GTK_CONTAINER (node)));
    }
    return NULL;
}

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

/* GomElement implementation */

static G_CONST_RETURN char *
widget_get_tag_name (GomElement *elem)
{
    return g_type_name (G_TYPE_FROM_INSTANCE (elem));
}

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
widget_element_init (gpointer g_iface, gpointer iface_data)
{
    GomElementInterface *elem = (GomElementInterface *)g_iface;

#define IFACE(func) elem->func = widget_##func

    IFACE (get_tag_name);

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

GOM_DEFINE_JS_OBJECT(widget, GomJSElementClass)

#undef GTK_TYPE_FILE_INFO
#undef GTK_TYPE_FILE_SYSTEM_HANDLE
#undef GTK_TYPE_FILE_SYSTEM
#undef GTK_TYPE_FILE_FOLDER
#undef GTK_TYPE_FILE_PATH
#undef GTK_TYPE_TEXT
#undef GTK_TYPE_TEXT_LAYOUT
#undef GTK_TYPE_TREE
#undef GTK_TYPE_TREE_ITEM

#define GTK_TYPE_FILE_INFO 0
#define GTK_TYPE_FILE_SYSTEM_HANDLE 0
#define GTK_TYPE_FILE_SYSTEM 0
#define GTK_TYPE_FILE_FOLDER 0
#define GTK_TYPE_FILE_PATH 0
#define GTK_TYPE_TEXT 0
#define GTK_TYPE_TEXT_LAYOUT 0
#define GTK_TYPE_TREE 0
#define GTK_TYPE_TREE_ITEM 0

static gpointer
gom_widget_init_once (gpointer data)
{
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
    static const GInterfaceInfo js_object_info = {
        widget_js_object_init,
        NULL, /* interface_finalize */
        NULL /* interface_data */
    };
    g_type_add_interface_static (GTK_TYPE_WIDGET, GOM_TYPE_NODE, &node_info);
    g_type_add_interface_static (GTK_TYPE_WIDGET, GOM_TYPE_ELEMENT, &element_info);
    g_type_add_interface_static (GTK_TYPE_WIDGET, GOM_TYPE_JS_OBJECT, &js_object_info);

#include "gomwidgets.c"

    return GSIZE_TO_POINTER (type);
}

void
gom_widget_init (void)
{
    static GOnce init_once = G_ONCE_INIT;
    g_once (&init_once, gom_widget_init_once, NULL);
}
