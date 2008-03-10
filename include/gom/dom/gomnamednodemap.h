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
#ifndef GOM_NAMED_NODE_MAP_H
#define GOM_NAMED_NODE_MAP_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomNamedNodeMap GomNamedNodeMap; /* dummy object */
typedef struct _GomNamedNodeMapInterface GomNamedNodeMapInterface;

G_END_DECLS

#include <glib-object.h>
#include <gom/dom/gomnode.h>

G_BEGIN_DECLS

#define GOM_TYPE_NAMED_NODE_MAP             (gom_named_node_map_get_type ())
#define GOM_NAMED_NODE_MAP(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_NAMED_NODE_MAP, GomNamedNodeMap))
#define GOM_IS_NAMED_NODE_MAP(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_NAMED_NODE_MAP))
#define GOM_NAMED_NODE_MAP_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_NAMED_NODE_MAP, GomNamedNodeMapInterface))

struct _GomNamedNodeMapInterface {
    GTypeInterface parent;

    GomNode *(*get_named_item)    (GomNamedNodeMap *map,
                                   const char      *name);
    GomNode *(*set_named_item)    (GomNamedNodeMap *map,
                                   GomNode         *arg,
                                   GError         **error);
    GomNode *(*remove_named_item) (GomNamedNodeMap *map,
                                   const char      *name,
                                   GError         **error);
    GomNode *(*item)              (GomNamedNodeMap *map,
                                   gulong           index);
};

GType gom_named_node_map_get_type (void);

GomNode *gom_named_node_map_get_named_item    (GomNamedNodeMap *map,
                                               const char      *name);
GomNode *gom_named_node_map_set_named_item    (GomNamedNodeMap *map,
                                               GomNode         *arg,
                                               GError         **error);
GomNode *gom_named_node_map_remove_named_item (GomNamedNodeMap *map,
                                               const char      *name,
                                               GError         **error);
GomNode *gom_named_node_map_item              (GomNamedNodeMap *map,
                                               gulong           index);

#endif /* GOM_NAMED_NODE_MAP_H */
