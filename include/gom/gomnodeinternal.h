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
#ifndef GOM_NODE_INTERNAL_H
#define GOM_NODE_INTERNAL_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomNodeInternal          GomNodeInternal; /* dummy object */
typedef struct _GomNodeInternalInterface GomNodeInternalInterface;

G_END_DECLS

#include <gom/dom/gomevent.h>
#include <gom/dom/gomnode.h>

G_BEGIN_DECLS

#define GOM_TYPE_NODE_INTERNAL             (gom_node_internal_get_type ())
#define GOM_NODE_INTERNAL(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_NODE_INTERNAL, GomNodeInternal))
#define GOM_IS_NODE_INTERNAL(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_NODE_INTERNAL))
#define GOM_NODE_INTERNAL_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_NODE_INTERNAL, GomNodeInternalInterface))

#define _GOM_IMPLEMENT_NODE_INTERNAL(i, p, f) (((GomNodeInternalInterface*)i)->f = p##_##f)
#define GOM_IMPLEMENT_NODE_INTERNAL(i, p)                        \
    G_STMT_START {                                               \
        _GOM_IMPLEMENT_NODE_INTERNAL (i, p, set_parent);         \
        _GOM_IMPLEMENT_NODE_INTERNAL (i, p, set_next_sibling);   \
        _GOM_IMPLEMENT_NODE_INTERNAL (i, p, set_prev_sibling);   \
        _GOM_IMPLEMENT_NODE_INTERNAL (i, p, sibling_requested);  \
        _GOM_IMPLEMENT_NODE_INTERNAL (i, p, dispatch_listeners); \
    } G_STMT_END

struct _GomNodeInternalInterface {
    GTypeInterface parent;

    void (*set_parent)         (GomNodeInternal *child,  GomNode *parent);
    void (*set_next_sibling)   (GomNodeInternal *child,  GomNode *sibling);
    void (*set_prev_sibling)   (GomNodeInternal *child,  GomNode *sibling);
    void (*sibling_requested)  (GomNodeInternal *parent, GomNode *child);
    void (*dispatch_listeners) (GomNodeInternal *current_target,
                                GomEvent        *evt,
                                const char      *namespace_uri,
                                const char      *type_name,
                                GomPhaseType     phase);
};

GType gom_node_internal_get_type (void);

void  gom_node_internal_set_parent         (GomNodeInternal *child, GomNode *parent);
void  gom_node_internal_set_next_sibling   (GomNodeInternal *child, GomNode *sibling);
void  gom_node_internal_set_prev_sibling   (GomNodeInternal *child, GomNode *sibling);
void  gom_node_internal_sibling_requested  (GomNodeInternal *parent, GomNode *child);

void  gom_node_internal_dispatch_listeners (GomNodeInternal *current_target,
                                            GomEvent        *evt,
                                            const char      *namespace_uri,
                                            const char      *type_name,
                                            GomPhaseType     phase);
G_END_DECLS

#endif /* GOM_NODE_INTERNAL_H */
