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
#ifndef GOM_PARENT_H
#define GOM_PARENT_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomParent GomParent; /* dummy object */
typedef struct _GomParentInterface GomParentInterface;

G_END_DECLS

#include <gom/dom/gomnode.h>

G_BEGIN_DECLS

#define GOM_TYPE_PARENT             (gom_parent_get_type ())
#define GOM_PARENT(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_PARENT, GomParent))
#define GOM_IS_PARENT(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_PARENT))
#define GOM_PARENT_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_PARENT, GomParentInterface))

#define _GOM_IMPLEMENT_PARENT(i, p, f) (((GomParentInterface*)i)->f = p##_##f)
#define GOM_IMPLEMENT_PARENT(i, p)                        \
    G_STMT_START {                                        \
        _GOM_IMPLEMENT_PARENT (i, p, sibling_requested);  \
    } G_STMT_END

struct _GomParentInterface {
    GTypeInterface parent;

    void (*sibling_requested) (GomParent *parent, GomNode *child);
};

GType gom_parent_get_type         (void);

void  gom_parent_sibling_requested (GomParent *parent, GomNode *child);

G_END_DECLS

#endif /* GOM_PARENT_H */
