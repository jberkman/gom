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

#include "gom/gomnoodle.h"

#include "gom/dom/gomdombuiltins.h"
#include "gom/dom/gomdocumenttype.h"
#include "gom/dom/gomeventtarget.h"
#include "gom/dom/gomnode.h"
#include "gom/gomchild.h"
#include "gom/gomglist.h"
#include "gom/gomparent.h"

#include "gommacros.h"

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
    char           *namespace_uri;
    char           *prefix;
    char           *local_name;
    char           *node_name;
    GList          *children;
    GomDocument    *owner_document;
    GomNode        *parent_node;
    GomNode        *prev_sibling;
    GomNode        *next_sibling;
    GomNodeType     node_type;
    guint           constructed    : 1;
    guint           dirty_children : 1;
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
    case PROP_NODE_NAME:
        g_value_set_string (value, priv->node_name);
        break;
    case PROP_NODE_VALUE:
        g_value_set_string (value, NULL);
        break;
    case PROP_NODE_TYPE:
        g_value_set_enum (value, priv->node_type);
        break;
    case PROP_OWNER_DOCUMENT:
        g_value_set_object (value, priv->owner_document);
        break;
    case PROP_PREVIOUS_SIBLING:
        if (GOM_IS_PARENT (priv->parent_node)) {
            gom_parent_sibling_requested (GOM_PARENT (priv->parent_node), GOM_NODE (object));
        }
        g_value_set_object (value, priv->prev_sibling);
        break;
    case PROP_NEXT_SIBLING:
        if (GOM_IS_PARENT (priv->parent_node)) {
            gom_parent_sibling_requested (GOM_PARENT (priv->parent_node), GOM_NODE (object));
        }
        g_value_set_object (value, priv->next_sibling);
        break;
    case PROP_PARENT_NODE:
        g_value_set_object (value, priv->parent_node);
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
    case PROP_NAMESPACE_URI:
        g_value_set_string (value, priv->namespace_uri);
        break;
    case PROP_PREFIX:
        g_value_set_string (value, priv->prefix);
        break;
    case PROP_LOCAL_NAME:
        g_value_set_string (value, priv->local_name);
        break;
    case PROP_ATTRIBUTES:
        g_value_set_string (value, NULL);
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
    case PROP_NODE_NAME:
        priv->node_name = g_value_dup_string (value);
        break;
    case PROP_NODE_TYPE:
        priv->node_type = g_value_get_enum (value);
        break;
    case PROP_OWNER_DOCUMENT:
        priv->owner_document = g_value_dup_object (value);
        break;
    case PROP_NAMESPACE_URI:
        priv->namespace_uri = g_value_dup_string (value);
        break;
    case PROP_LOCAL_NAME:
        priv->local_name = g_value_dup_string (value);
        break;
    case PROP_PREFIX:
        g_free (priv->prefix);
        priv->prefix = g_value_dup_string (value);
        if (priv->local_name) {
            g_free (priv->node_name);
            priv->node_name = priv->prefix 
                ? g_strconcat (priv->prefix, ":", priv->local_name, NULL)
                : g_strdup (priv->local_name);
        }
        break;
    case PROP_NODE_VALUE:
        GOM_NOT_IMPLEMENTED;
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static GList *
find_node (GomNode *node, GomNode *child, GError **error)
{
    GomNoodlePrivate *priv = PRIV (node);
    GomNode *parent;
    GList *li = NULL;
    char *child_name;
    g_object_get (child, "parent-node", &parent, NULL);
    if (parent == node) {
        li = g_list_find (priv->children, child);
    }
    if (parent) {
        g_object_unref (parent);
    }
    if (!li) {
        g_object_get (child, "node-name", &child_name, NULL);
        g_set_error (error,
                     GOM_DOM_EXCEPTION_ERROR,
                     GOM_NOT_FOUND_ERR,
                     "child <%s %p> is not a child of <%s %p>",
                     child_name, child,
                     priv->node_name, node);
        g_free (child_name);
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
        if (GOM_IS_CHILD (new_child)) {
            gom_child_set_parent (GOM_CHILD (new_child), node);
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
        if (GOM_IS_CHILD (ref_child)) {
            gom_child_set_parent (GOM_CHILD (ref_child), NULL);
        }
        g_object_unref (ref_child);
        if (GOM_IS_CHILD (new_child)) {
            gom_child_set_parent (GOM_CHILD (new_child), node);
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
        if (GOM_IS_CHILD (old_child)) {
            gom_child_set_parent (GOM_CHILD (old_child), NULL);
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
    if (GOM_IS_CHILD (new_child)) {
        gom_child_set_parent (GOM_CHILD (new_child), node);
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
    g_object_get (PRIV (node)->owner_document, "implementation", &dom, NULL);
    return gom_dom_implementation_has_feature (dom, feature, version);
}

static gboolean
gom_noodle_has_attributes (GomNode *node)
{
    GOM_NOT_IMPLEMENTED;
    return FALSE;
}                         

static void
gom_noodle_set_parent (GomChild *child, GomNode *parent)
{
    GomNoodlePrivate *priv = PRIV (child);
    char *child_name, *parent_name = NULL;
    g_object_get (child, "node-name", &child_name, NULL);
    if (parent) {
        g_object_get (parent, "node-name", &parent_name, NULL);
    }
    g_print ("%s:%d:%s(<%s %p>, <%s %p>)\n",
             __FILE__, __LINE__, __FUNCTION__,
             child_name, child, 
             parent_name ? parent_name : "(no parent)", parent);
    g_free (child_name);
    g_free (parent_name);
    priv->next_sibling = NULL;
    priv->prev_sibling = NULL;
    priv->parent_node  = parent;
}

static void
gom_noodle_set_prev_sibling (GomChild *child, GomNode *sibling)
{
    char *child_name, *sibling_name = NULL;
    g_object_get (child, "node-name", &child_name, NULL);
    if (sibling) {
        g_object_get (sibling, "node-name", &sibling_name, NULL);
    }
    g_print ("%s:%d:%s(<%s %p>, <%s %p>)\n",
             __FILE__, __LINE__, __FUNCTION__,
             child_name, child,
             sibling_name? sibling_name : "(no sibling)", sibling);
    g_free (child_name);
    g_free (sibling_name);
    PRIV (child)->prev_sibling = sibling;
}

static void
gom_noodle_set_next_sibling (GomChild *child, GomNode *sibling)
{
    char *child_name, *sibling_name = NULL;
    g_object_get (child, "node-name", &child_name, NULL);
    if (sibling) {
        g_object_get (sibling, "node-name", &sibling_name, NULL);
    }
    g_print ("%s:%d:%s(<%s %p>, <%s %p>)\n",
             __FILE__, __LINE__, __FUNCTION__,
             child_name, child,
             sibling_name ? sibling_name : "(no sibling)", sibling);
    g_free (child_name);
    g_free (sibling_name);
    PRIV (child)->next_sibling = sibling;
}

static void
gom_noodle_sibling_requested (GomParent *node, GomNode *child)
{
    GomNoodlePrivate *priv = PRIV (node);
    GList *li;
    if (!priv->dirty_children) {
        return;
    }
    for (li = priv->children; li; li = li->next) {
        if (GOM_IS_CHILD (li->data)) {
            gom_child_set_prev_sibling (li->data, li->prev ? li->prev->data : NULL);
            gom_child_set_next_sibling (li->data, li->next ? li->next->data : NULL);
        }
    }
    priv->dirty_children = 0;
}

GOM_IMPLEMENT (NODE,   node,   gom_noodle);
GOM_IMPLEMENT (CHILD,  child,  gom_noodle);
GOM_IMPLEMENT (PARENT, parent, gom_noodle);

G_DEFINE_TYPE_WITH_CODE (GomNoodle, gom_noodle, GOM_TYPE_TARGET,
                         {
                             GOM_IMPLEMENT_INTERFACE (NODE,   node,   gom_noodle);
                             GOM_IMPLEMENT_INTERFACE (CHILD,  child,  gom_noodle);
                             GOM_IMPLEMENT_INTERFACE (PARENT, parent, gom_noodle);
                         });

static void gom_noodle_init (GomNoodle *noodle) { }

static void
gom_noodle_finalize (GObject *object)
{
    GomNoodlePrivate *priv = PRIV (object);
    g_print ("%s:%d:%s (%s %p)\n",
             __FILE__, __LINE__, __FUNCTION__,
             g_type_name (G_TYPE_FROM_INSTANCE (object)), object);

    g_free (priv->namespace_uri);
    priv->namespace_uri = NULL;

    g_free (priv->prefix);
    priv->prefix = NULL;

    g_free (priv->local_name);
    priv->local_name = NULL;

    g_free (priv->node_name);
    priv->node_name = NULL;

    g_list_foreach (priv->children, (GFunc)g_object_unref, NULL);
    g_free (priv->children);
    priv->children = NULL;

    if (priv->owner_document) {
        g_object_unref (priv->owner_document);
        priv->owner_document = NULL;
    }

    G_OBJECT_CLASS (gom_noodle_parent_class)->finalize (object);
}

static void
gom_noodle_constructed (GObject *object)
{
    GomNoodlePrivate *priv = PRIV (object);

    if (G_OBJECT_CLASS (gom_noodle_parent_class)->constructed) {
        G_OBJECT_CLASS (gom_noodle_parent_class)->constructed (object);
    }

    priv->constructed = 1;

    if (!priv->owner_document) {
        g_warning ("%s:%d:%s(%s %p): No document set",
                   __FILE__, __LINE__, __FUNCTION__,
                   g_type_name (G_TYPE_FROM_INSTANCE (object)),
                   object);
    }

    if (!priv->local_name && !priv->node_name) {
        g_warning ("%s:%d:%s(%s %p): No localName or nodeName set",
                   __FILE__, __LINE__, __FUNCTION__,
                   g_type_name (G_TYPE_FROM_INSTANCE (object)),
                   object);
        return;
    }

    if (priv->local_name) {
        if (!priv->node_name) {
            priv->node_name = priv->prefix 
                ? g_strconcat (priv->prefix, ":", priv->local_name, NULL)
                : g_strdup (priv->local_name);
        }
    } else {
        const char *colon = strchr (priv->node_name, ':');
        priv->local_name = g_strdup (colon ? colon + 1 : priv->node_name);
        if (colon) {
            if (priv->prefix) {
                if (colon - priv->node_name != strlen (priv->prefix) ||
                    strncmp (priv->prefix, priv->node_name, colon - priv->node_name)) {
                    g_warning ("%s:%d:%s(%s %p): Prefix %s doesn't agree with nodeName %s",
                               __FILE__, __LINE__, __FUNCTION__,
                               g_type_name (G_TYPE_FROM_INSTANCE (object)), object,
                               priv->prefix, priv->node_name);
                }
            } else {
                priv->prefix = g_strndup (priv->node_name, colon - priv->node_name);
            }
        }
    }

    g_print ("%s:%d:%s(%s %p): namespaceURI: %s prefix: %s localName: %s nodeName: %s\n",
             __FILE__, __LINE__, __FUNCTION__,
             g_type_name (G_TYPE_FROM_INSTANCE (object)),
             object,
             priv->namespace_uri,
             priv->prefix,
             priv->local_name,
             priv->node_name);
}

static void
gom_noodle_class_init (GomNoodleClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (GomNoodlePrivate));

    oclass->constructed  = gom_noodle_constructed;
    oclass->finalize     = gom_noodle_finalize;
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

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomChild, gom_child, {}, GOM_TYPE_NODE);

GOM_STUB_VOID (GOM_CHILD, gom_child, set_parent,
               (GomChild *gom_child, GomNode *parent), (gom_child, parent));

GOM_STUB_VOID (GOM_CHILD, gom_child, set_next_sibling,
               (GomChild *gom_child, GomNode *sibling), (gom_child, sibling));

GOM_STUB_VOID (GOM_CHILD, gom_child, set_prev_sibling,
               (GomChild *gom_child, GomNode *sibling), (gom_child, sibling));

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomParent, gom_parent, {}, GOM_TYPE_NODE);

GOM_STUB_VOID (GOM_PARENT, gom_parent, sibling_requested,
               (GomParent *gom_parent, GomNode *child), (gom_parent, child));
