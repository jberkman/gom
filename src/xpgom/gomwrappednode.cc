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

#include "xpgom/gomwrappednode.hh"
#include "gom/dom/gomnode.h"
#include "xpgom/xgNode.hh"
#include "gom/dom/gomdomexception.h"
#include "gom/dom/gomnamednodemap.h"

#include <nsIDOMNode.h>
#include <nsCOMPtr.h>
#include <nsIDOMDocument.h>
#include <nsIDOMNamedNodeMap.h>
#include <nsStringAPI.h>
#include <nsIDOMNodeList.h>

#include "gommacros.h"

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

#define GET_NODE(i) GOM_WRAPPED_GET (i, nsIDOMNode, mNode)

static void
gom_wrapped_node_get_property (GObject    *object,
			       guint       property_id,
			       GValue     *value,
			       GParamSpec *pspec)
{
    GET_NODE (object);

    switch (property_id) {
    case PROP_OWNER_DOCUMENT:
	GOM_WRAPPED_GET_OBJECT (mNode, GetOwnerDocument, nsIDOMDocument, GOM_TYPE_DOCUMENT, GomDocument);
	break;
    case PROP_NODE_TYPE:
	GOM_WRAPPED_GET_ENUM (mNode, GetNodeType);
	break;
    case PROP_NAMESPACE_URI:
	GOM_WRAPPED_GET_STRING (mNode, GetNamespaceURI);
	break;
    case PROP_NODE_NAME:
	GOM_WRAPPED_GET_STRING (mNode, GetNodeName);
	break;
    case PROP_FIRST_CHILD:
	GOM_WRAPPED_GET_OBJECT (mNode, GetFirstChild, nsIDOMNode, GOM_TYPE_NODE, GomNode);
	break;
    case PROP_NEXT_SIBLING:
	GOM_WRAPPED_GET_OBJECT (mNode, GetNextSibling, nsIDOMNode, GOM_TYPE_NODE, GomNode);
	break;
    case PROP_NODE_VALUE:
	GOM_WRAPPED_GET_STRING (mNode, GetNodeValue);
	break;
    case PROP_ATTRIBUTES:
	GOM_WRAPPED_GET_OBJECT (mNode, GetAttributes, nsIDOMNamedNodeMap, GOM_TYPE_NAMED_NODE_MAP, GomNamedNodeMap);
	break;
    case PROP_PREVIOUS_SIBLING:
	GOM_WRAPPED_GET_OBJECT (mNode, GetPreviousSibling, nsIDOMNode, GOM_TYPE_NODE, GomNode);
	break;
    case PROP_PARENT_NODE:
	GOM_WRAPPED_GET_OBJECT (mNode, GetParentNode, nsIDOMNode, GOM_TYPE_NODE, GomNode);
	break;
    case PROP_CHILD_NODES:
	GOM_WRAPPED_GET_OBJECT (mNode, GetChildNodes, nsIDOMNodeList, GOM_TYPE_NODE_LIST, GomNodeList);
	break;
    case PROP_LAST_CHILD:
	GOM_WRAPPED_GET_OBJECT (mNode, GetLastChild, nsIDOMNode, GOM_TYPE_NODE, GomNode);
	break;
    case PROP_PREFIX:
	GOM_WRAPPED_GET_STRING (mNode, GetPrefix);
	break;
    case PROP_LOCAL_NAME:
	GOM_WRAPPED_GET_STRING (mNode, GetLocalName);
	break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
gom_wrapped_node_set_property (GObject *object,
			       guint property_id,
			       const GValue *value,
			       GParamSpec *pspec)
{
    GET_NODE (object);

    switch (property_id) {
    case PROP_PREFIX:
	GOM_PROPERTY_NOT_IMPLEMENTED (pspec);
	break;	
    case PROP_NAMESPACE_URI:
    case PROP_LOCAL_NAME:
    case PROP_OWNER_DOCUMENT:
	/* */
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static GomNode *
gom_wrapped_node_insert_before (GomNode *node, GomNode *new_child, GomNode *ref_child, GError **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomNode *
gom_wrapped_node_replace_child (GomNode *node, GomNode *new_child, GomNode *ref_child, GError **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomNode *
gom_wrapped_node_remove_child  (GomNode *node, GomNode *old_child, GError **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomNode *
gom_wrapped_node_append_child  (GomNode  *node, GomNode  *new_child, GError   **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static gboolean
gom_wrapped_node_has_child_nodes (GomNode *node)
{
    GOM_NOT_IMPLEMENTED;
    return FALSE;
}

static GomNode *
gom_wrapped_node_clone_node (GomNode *node, gboolean deep)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static void
gom_wrapped_node_normalize (GomNode *node)
{
    GOM_NOT_IMPLEMENTED;
}

static gboolean
gom_wrapped_node_is_supported (GomNode    *node,
			       const char *feature,
			       const char *version)
{
    GOM_NOT_IMPLEMENTED;
    return FALSE;
}

static gboolean
gom_wrapped_node_has_attributes (GomNode *node)
{
    GOM_NOT_IMPLEMENTED;
    return FALSE;
}

GOM_WRAPPED_CONSTRUCTOR_INIT (xgNode, Init)

GOM_IMPLEMENT (NODE, node, gom_wrapped_node);

G_DEFINE_TYPE_WITH_CODE (GomWrappedNode, gom_wrapped_node, GOM_TYPE_WRAPPED,
			 GOM_IMPLEMENT_INTERFACE (NODE, node, gom_wrapped_node));

static void gom_wrapped_node_init (GomWrappedNode *node) { }

static void
gom_wrapped_node_class_init (GomWrappedNodeClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    oclass->get_property = gom_wrapped_node_get_property;
    oclass->set_property = gom_wrapped_node_set_property;

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

    g_object_class_install_property (
        oclass,
	PROP_OWNER_DOCUMENT,
        g_param_spec_object ("owner-document", NULL,
                             "The Document object associated with this node.",
                             GOM_TYPE_DOCUMENT,
                             GParamFlags (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS)));
    
    g_object_class_install_property (
        oclass,
	PROP_NAMESPACE_URI,
        g_param_spec_string ("namespace-u-r-i", "namespace-uri",
                             "The namespace URI of this node, or null if it is unspecified.",
                             NULL,
                             GParamFlags (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS)));

    g_object_class_install_property (
        oclass,
	PROP_PREFIX,
        g_param_spec_string ("prefix", NULL,
                             "The namespace prefix of this node, or null if it is unspecified.",
                             NULL,
                             GParamFlags (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS)));

    g_object_class_install_property (
        oclass,
	PROP_LOCAL_NAME,
        g_param_spec_string ("local-name", NULL,
                             "Returns the local part of the qualified name of this node.",
                             NULL,
                             GParamFlags (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS)));

    gom_wrapped_register_interface (GOM_TYPE_NODE, GOM_TYPE_WRAPPED_NODE,
				    NS_GET_IID (nsIDOMNode), xgNodeConstructor);
}
