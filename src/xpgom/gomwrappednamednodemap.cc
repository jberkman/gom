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

#include "xpgom/gomwrappednamednodemap.hh"
#include "gom/dom/gomnamednodemap.h"
#if 0
#include "xpgom/xgNode.hh"
#endif
#include "gom/dom/gomdomexception.h"

#include <nsCOMPtr.h>
#include <nsIDOMNamedNodeMap.h>
#include <nsIDOMNode.h>
#include <nsStringAPI.h>

#include "gommacros.h"

enum {
    PROP_LENGTH = 1,
};

#define GET_MAP(i) GOM_WRAPPED_GET (i, nsIDOMNamedNodeMap, mMap)

static void
gom_wrapped_named_node_map_get_property (GObject    *object,
					 guint       property_id,
					 GValue     *value,
					 GParamSpec *pspec)
{
    GET_MAP (object);
    switch (property_id) {
    case PROP_LENGTH:
	GOM_WRAPPED_GET_ULONG (mMap, GetLength);
	break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static GomNode *
gom_wrapped_named_node_map_get_named_item (GomNamedNodeMap *map,
					   const char      *name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomNode *
gom_wrapped_named_node_map_set_named_item (GomNamedNodeMap *map,
                                               GomNode         *arg,
                                               GError         **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomNode *
gom_wrapped_named_node_map_remove_named_item (GomNamedNodeMap *map,
					      const char      *name,
					      GError         **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomNode *
gom_wrapped_named_node_map_item (GomNamedNodeMap *map,
				 gulong           index)
{
    GET_MAP (map);
    nsCOMPtr<nsIDOMNode> aNode;
    if (NS_FAILED (mMap->Item (index, getter_AddRefs (aNode)))) {
	return NULL;
    }
    return (GomNode *)gom_wrap_xpcom (aNode, GOM_TYPE_NODE, NULL);
}

static GomNode *
gom_wrapped_named_node_map_get_named_item_ns (GomNamedNodeMap *map,
					      const char      *namespace_uri,
					      const char      *local_name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomNode *
gom_wrapped_named_node_map_set_named_item_ns (GomNamedNodeMap *map,
					      GomNode         *arg,
					      GError         **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomNode *
gom_wrapped_named_node_map_remove_named_item_ns (GomNamedNodeMap *map,
						 const char      *namespace_uri,
						 const char      *local_name,
						 GError         **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

// GOM_WRAPPED_CONSTRUCTOR_INIT (xgNode, Init)

GOM_IMPLEMENT (NAMED_NODE_MAP, named_node_map, gom_wrapped_named_node_map)

G_DEFINE_TYPE_WITH_CODE (GomWrappedNamedNodeMap, gom_wrapped_named_node_map, GOM_TYPE_WRAPPED,
			 GOM_IMPLEMENT_INTERFACE (NAMED_NODE_MAP, named_node_map, gom_wrapped_named_node_map))

static void gom_wrapped_named_node_map_init (GomWrappedNamedNodeMap *node) { }

static void
gom_wrapped_named_node_map_class_init (GomWrappedNamedNodeMapClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    oclass->get_property = gom_wrapped_named_node_map_get_property;

    g_object_class_override_property (oclass, PROP_LENGTH, "length");

    gom_wrapped_register_interface (GOM_TYPE_NAMED_NODE_MAP, GOM_TYPE_WRAPPED_NAMED_NODE_MAP,
				    NS_GET_IID (nsIDOMNamedNodeMap), NULL /*xgNamedNodeMapConstructor*/);
}
