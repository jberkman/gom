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

#include "gom/dom/gomnode.h"

#include "gom/gomnoodle.h"

#include "gom/dom/gomdocumenttype.h"
#include "gom/dom/gomdombuiltins.h"
#include "gom/dom/gomeventtarget.h"
#include "gom/gomglist.h"
#include "gom/gomnodeinternal.h"

#include "gommacros.h"

#include <gtk/gtkwindow.h>
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
    PROP_NAMESPACE_URI,
    PROP_PREFIX,
    PROP_LOCAL_NAME,
};

typedef struct {
    GList           *children;
    GomDocument     *owner_document;
    GomNode         *parent_node;
    GomNode         *prev_sibling;
    GomNode         *next_sibling;
    guint            constructed    : 1;
    guint            dirty_children : 1;
} GomNoodlePrivate;

#define PRIV(i) G_TYPE_INSTANCE_GET_PRIVATE ((i), GOM_TYPE_NOODLE, GomNoodlePrivate)

static void
gom_noodle_get_property (GObject    *object,
                         guint       property_id,
                         GValue     *value,
                         GParamSpec *pspec)
{
    GomNoodlePrivate *priv = PRIV (object);

    switch (property_id) {
    case PROP_OWNER_DOCUMENT:
        g_value_set_object (value, priv->owner_document);
        break;
    case PROP_PREVIOUS_SIBLING:
        if (GOM_IS_NODE_INTERNAL (priv->parent_node)) {
            gom_node_internal_sibling_requested (GOM_NODE_INTERNAL (priv->parent_node), GOM_NODE (object));
        }
        g_value_set_object (value, priv->prev_sibling);
        break;
    case PROP_NEXT_SIBLING:
        if (GOM_IS_NODE_INTERNAL (priv->parent_node)) {
            gom_node_internal_sibling_requested (GOM_NODE_INTERNAL (priv->parent_node), GOM_NODE (object));
        }
        g_value_set_object (value, priv->next_sibling);
        break;
    case PROP_PARENT_NODE:
        g_value_set_object (value, priv->parent_node);
        break;
    case PROP_CHILD_NODES: {
	GomNodeList *nodes;
	nodes = g_object_new (GOM_TYPE_G_LIST, "nodes", priv->children, NULL);
        g_value_set_object (value, nodes);
	g_object_unref (nodes);
        break;
    }
    case PROP_FIRST_CHILD:
        g_value_set_object (value, priv->children ? priv->children->data : NULL);
        break;
    case PROP_LAST_CHILD: {
        GList *last;
        last = g_list_last (priv->children);
        g_value_set_object (value, last ? last->data : NULL);
        break;
    }
    case PROP_NAMESPACE_URI:
    case PROP_PREFIX:
    case PROP_LOCAL_NAME:
    case PROP_NODE_VALUE:
        g_value_set_string (value, NULL);
        break;
    case PROP_ATTRIBUTES:
        g_value_set_object (value, NULL);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
gom_noodle_set_property (GObject *object,
                         guint property_id,
                         const GValue *value,
                         GParamSpec *pspec)
{
    GomNoodlePrivate *priv = PRIV (object);

    switch (property_id) {
    case PROP_OWNER_DOCUMENT:
        GOM_SET_WEAK (priv->owner_document, g_value_get_object (value));
        break;
    case PROP_NAMESPACE_URI:
    case PROP_PREFIX:
    case PROP_LOCAL_NAME:
        if (!g_value_get_string (value)) {
            break;
        }
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static GomEventTarget *
gom_noodle_get_parent_target (GomEventTargetInternal *target)
{
    GomNoodlePrivate *priv = PRIV (target);

    return priv->parent_node ? GOM_EVENT_TARGET (priv->parent_node) : NULL;
}

#define gom_noodle_dispatch_listeners gom_evt_tgt_dispatch_listeners

static GList *
find_node (GomNode *node, GomNode *child, GError **error)
{
    GomNoodlePrivate *priv = PRIV (node);
    GomNode *parent;
    GList *li = NULL;

    g_object_get (child, "parent-node", &parent, NULL);
    if (parent == node) {
        li = g_list_find (priv->children, child);
    }
    if (parent) {
        g_object_unref (parent);
    }
    if (!li) {
        char *child_name = NULL;
        char *node_name  = NULL;
        g_object_get (child, "node-name", &child_name, NULL);
        g_object_get (node,  "node-name", &node_name,  NULL);
        g_set_error (error,
                     GOM_DOM_EXCEPTION_ERROR,
                     GOM_NOT_FOUND_ERR,
                     "child <%s %p> is not a child of <%s %p>",
                     child_name, child,
                     node_name, node);
        g_free (child_name);
        g_free (node_name);
    }
    return li;
}

static GomNode *
gom_noodle_insert_before (GomNode *node,
                          GomNode *new_child,
                          GomNode *ref_child,
                          GError  **error)
{
    GomNoodlePrivate *priv = PRIV (node);
    GList *li;
    li = find_node (node, ref_child, error);
    if (li) {
        priv->children = g_list_insert_before (priv->children, li, g_object_ref (new_child));
        priv->dirty_children = 1;
        if (GOM_IS_NODE_INTERNAL (new_child)) {
            gom_node_internal_set_parent (GOM_NODE_INTERNAL (new_child), node);
        }
    }
    return ref_child;
}

static GomNode *
gom_noodle_replace_child (GomNode *node,
                          GomNode *new_child,
                          GomNode *ref_child,
                          GError  **error)
{
    GomNoodlePrivate *priv = PRIV (node);
    GList *li;
    li = find_node (node, ref_child, error);
    if (li) {
        li->data = g_object_ref (new_child);
        priv->dirty_children = 1;
        if (GOM_IS_NODE_INTERNAL (ref_child)) {
            gom_node_internal_set_parent (GOM_NODE_INTERNAL (ref_child), NULL);
        }
        g_object_unref (ref_child);
        if (GOM_IS_NODE_INTERNAL (new_child)) {
            gom_node_internal_set_parent (GOM_NODE_INTERNAL (new_child), node);
        }
    }
    return ref_child;
}

static GomNode *
gom_noodle_remove_child (GomNode *node,
                         GomNode *old_child,
                         GError  **error)
{
    GomNoodlePrivate *priv = PRIV (node);
    GList *li;
    li = find_node (node, old_child, error);
    if (li) {
        if (GOM_IS_NODE_INTERNAL (old_child)) {
            gom_node_internal_set_parent (GOM_NODE_INTERNAL (old_child), NULL);
        }
        g_object_unref (old_child);
        priv->children = g_list_delete_link (priv->children, li);
        priv->dirty_children = 1;
    }
    return old_child;
}

static GomNode *
gom_noodle_append_child (GomNode *node,
                         GomNode *new_child,
                         GError  **error)
{
    GomNoodlePrivate *priv = PRIV (node);
    GomNode *parent;
    GError *err = NULL;
    g_object_get (new_child, "parent-node", &parent, NULL);
    if (parent) {
        gom_node_remove_child (parent, new_child, &err);
        g_object_unref (parent);
        if (err) {
            g_propagate_error (error, err);
            return new_child;
        }
    }
    priv->children = g_list_append (priv->children, g_object_ref (new_child));
    priv->dirty_children = 1;
    if (GOM_IS_NODE_INTERNAL (new_child)) {
        gom_node_internal_set_parent (GOM_NODE_INTERNAL (new_child), node);
    }
    return new_child;
}

static gboolean
gom_noodle_has_child_nodes (GomNode *node)
{
    GomNoodlePrivate *priv = PRIV (node);
    return g_list_length (priv->children) != 0;
}

static GomNode *
gom_noodle_clone_node (GomNode *node, gboolean deep)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static void
gom_noodle_normalize (GomNode *node)
{
    GOM_NOT_IMPLEMENTED;
}

static gboolean
gom_noodle_is_supported (GomNode    *node,
                         const char *feature,
                         const char *version)
{
    GomDOMImplementation *dom;
    gboolean ret;
    g_object_get (PRIV (node)->owner_document, "implementation", &dom, NULL);
    ret = gom_dom_implementation_has_feature (dom, feature, version);
    g_object_unref (dom);
    return ret;
}

static gboolean
gom_noodle_has_attributes (GomNode *node)
{
    GOM_NOT_IMPLEMENTED;
    return FALSE;
}                         

static void
gom_noodle_set_parent (GomNodeInternal *child, GomNode *parent)
{
    GomNoodlePrivate *priv = PRIV (child);
#if 0
    char *child_name, *parent_name = NULL;
    g_object_get (child, "node-name", &child_name, NULL);
    if (parent) {
        g_object_get (parent, "node-name", &parent_name, NULL);
    }
    g_print (GOM_LOC ("<%s %p> <%s %p>\n"),
              child_name, child, 
             parent_name ? parent_name : "(no parent)", parent);
    g_free (child_name);
    g_free (parent_name);
#endif
    GOM_SET_WEAK (priv->next_sibling, NULL);
    GOM_SET_WEAK (priv->prev_sibling, NULL);
    GOM_SET_WEAK (priv->parent_node, parent);
}

static void
gom_noodle_set_prev_sibling (GomNodeInternal *child, GomNode *sibling)
{
    GomNoodlePrivate *priv = PRIV (child);
#if 0
    char *child_name, *sibling_name = NULL;
    g_object_get (child, "node-name", &child_name, NULL);
    if (sibling) {
        g_object_get (sibling, "node-name", &sibling_name, NULL);
    }
    g_print (GOM_LOC ("<%s %p> <%s %p>\n"),
              child_name, child,
             sibling_name? sibling_name : "(no sibling)", sibling);
    g_free (child_name);
    g_free (sibling_name);
#endif
    GOM_SET_WEAK (priv->prev_sibling, sibling);
}

static void
gom_noodle_set_next_sibling (GomNodeInternal *child, GomNode *sibling)
{
    GomNoodlePrivate *priv = PRIV (child);
#if 0
    char *child_name, *sibling_name = NULL;
    g_object_get (child, "node-name", &child_name, NULL);
    if (sibling) {
        g_object_get (sibling, "node-name", &sibling_name, NULL);
    }
    g_print (GOM_LOC ("<%s %p> <%s %p>\n"),
             child_name, child,
             sibling_name ? sibling_name : "(no sibling)", sibling);
    g_free (child_name);
    g_free (sibling_name);
#endif
    GOM_SET_WEAK (priv->next_sibling, sibling);
}

static void
gom_noodle_sibling_requested (GomNodeInternal *node, GomNode *child)
{
    GomNoodlePrivate *priv = PRIV (node);
    GList *li;
    if (!priv->dirty_children) {
        return;
    }
    for (li = priv->children; li; li = li->next) {
        if (GOM_IS_NODE_INTERNAL (li->data)) {
            gom_node_internal_set_prev_sibling (li->data, li->prev ? li->prev->data : NULL);
            gom_node_internal_set_next_sibling (li->data, li->next ? li->next->data : NULL);
        }
    }
    priv->dirty_children = 0;
}

GOM_IMPLEMENT (EVENT_TARGET_INTERNAL, event_target_internal, gom_noodle);
GOM_IMPLEMENT (NODE,                  node,                  gom_noodle);
GOM_IMPLEMENT (NODE_INTERNAL,         node_internal,         gom_noodle);

G_DEFINE_TYPE_WITH_CODE (GomNoodle, gom_noodle, GOM_TYPE_EVT_TGT,
                         {
                             GOM_IMPLEMENT_INTERFACE (EVENT_TARGET,  event_target_internal,  gom_noodle);
                             GOM_IMPLEMENT_INTERFACE (NODE,          node,                   gom_noodle);
                             GOM_IMPLEMENT_INTERFACE (NODE_INTERNAL, node_internal,          gom_noodle);
                         });

static void gom_noodle_init (GomNoodle *noodle) { }

static void
unref_child (gpointer data, gpointer user_data)
{
    g_print ("refcnt %s -> %d\n", g_type_name (G_TYPE_FROM_INSTANCE (data)), G_OBJECT (data)->ref_count - 1);
    if (GTK_IS_WINDOW (data)) {
        gtk_object_destroy (GTK_OBJECT (data));
    } else {
        g_object_unref (data);
    }
}

static void
gom_noodle_dispose (GObject *object)
{
    GomNoodlePrivate *priv = PRIV (object);

#if 0
    g_print (GOM_LOC ("%s %p\n"),
             g_type_name (G_TYPE_FROM_INSTANCE (object)), object);
#endif
    g_list_foreach (priv->children, unref_child, NULL);
    g_list_free (priv->children);
    priv->children = NULL;

    GOM_UNSET_WEAK (priv->prev_sibling);
    GOM_UNSET_WEAK (priv->next_sibling);
    GOM_UNSET_WEAK (priv->parent_node);
    GOM_UNSET_WEAK (priv->owner_document);

    G_OBJECT_CLASS (gom_noodle_parent_class)->dispose (object);
}

static void
gom_noodle_constructed (GObject *object)
{
    GomNoodlePrivate *priv = PRIV (object);

    if (G_OBJECT_CLASS (gom_noodle_parent_class)->constructed) {
        G_OBJECT_CLASS (gom_noodle_parent_class)->constructed (object);
    }

    if (!priv->owner_document) {
        g_warning (GOM_LOC ("%s %p: No document set"),
                   g_type_name (G_TYPE_FROM_INSTANCE (object)),
                   object);
    }

    priv->constructed = 1;

}

static void
gom_noodle_class_init (GomNoodleClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (GomNoodlePrivate));

    oclass->constructed  = gom_noodle_constructed;
    oclass->dispose      = gom_noodle_dispose;
    oclass->get_property = gom_noodle_get_property;
    oclass->set_property = gom_noodle_set_property;

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
    g_object_class_override_property (oclass, PROP_NAMESPACE_URI,    "namespace-u-r-i");
    g_object_class_override_property (oclass, PROP_PREFIX,           "prefix");
    g_object_class_override_property (oclass, PROP_LOCAL_NAME,       "local-name");
}

