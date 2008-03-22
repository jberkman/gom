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
#ifndef GOM_NODE_H
#define GOM_NODE_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomNode GomNode; /* dummy object */
typedef struct _GomNodeInterface GomNodeInterface;

G_END_DECLS

#include <glib-object.h>

#include <gom/dom/gomnodelist.h>
#include <gom/dom/gomnamednodemap.h>
#include <gom/dom/gomdocument.h>

G_BEGIN_DECLS

#define GOM_TYPE_NODE             (gom_node_get_type ())
#define GOM_NODE(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_NODE, GomNode))
#define GOM_IS_NODE(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_NODE))
#define GOM_NODE_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_NODE, GomNodeInterface))

typedef enum {
    GOM_ELEMENT_NODE = 1,
    GOM_ATTRIBUTE_NODE = 2,
    GOM_TEXT_NODE = 3,
    GOM_CDATA_SECTION_NODE = 4,
    GOM_ENTITY_REFERENCE_NODE = 5,
    GOM_ENTITY_NODE = 6,
    GOM_PROCESSING_INSTRUCTION_NODE = 7,
    GOM_COMMENT_NODE = 8,
    GOM_DOCUMENT_NODE = 9,
    GOM_DOCUMENT_TYPE_NODE = 10,
    GOM_DOCUMENT_FRAGMENT_NODE = 11,
    GOM_NOTATION_NODE = 12
} GomNodeType;

struct _GomNodeInterface {
    GTypeInterface parent;
    
    GomNode   *(*insert_before)        (GomNode  *node,
                                         GomNode  *new_child,
                                         GomNode  *ref_child,
                                         GError   **error);

    GomNode   *(*replace_child)        (GomNode  *node,
                                         GomNode  *new_child,
                                         GomNode  *ref_child,
                                         GError   **error);

    GomNode   *(*remove_child)         (GomNode  *node,
                                         GomNode  *old_child,
                                         GError   **error);

    GomNode   *(*append_child)         (GomNode  *node,
                                         GomNode  *new_child,
                                         GError   **error);

    gboolean   (*has_child_nodes)      (GomNode *node);

    GomNode   *(*clone_node)           (GomNode *node,
                                        gboolean deep);

    void       (*normalize)            (GomNode *node);

    gboolean   (*is_supported)         (GomNode    *node,
                                        const char *feature,
                                        const char *version);

    gboolean   (*has_attributes)       (GomNode *node);
};

GType gom_node_get_type (void);

GomNode *gom_node_insert_before (GomNode *node, GomNode *new_child, GomNode *ref_child, GError **error);
GomNode *gom_node_replace_child (GomNode *node, GomNode *new_child, GomNode *ref_child, GError **error);
GomNode *gom_node_remove_child  (GomNode *node, GomNode *old_child, GError **error);
GomNode *gom_node_append_child  (GomNode  *node, GomNode  *new_child, GError   **error);

gboolean  gom_node_has_child_nodes (GomNode *node);

GomNode *gom_node_clone_node (GomNode *node, gboolean deep);

void     gom_node_normalize (GomNode *node);

gboolean gom_node_is_supported (GomNode    *node,
                                const char *feature,
                                const char *version);

gboolean gom_node_has_attributes (GomNode *node);

G_END_DECLS

#endif /* GOM_NODE_H */
