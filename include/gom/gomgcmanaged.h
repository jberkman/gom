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
#ifndef GOM_GC_MANAGED_H
#define GOM_GC_MANAGED_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomGCManaged          GomGCManaged; /* dummy object */
typedef struct _GomGCManagedInterface GomGCManagedInterface;

G_END_DECLS

#include <jsapi.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define GOM_TYPE_GC_MANAGED             (gom_gc_managed_get_type ())
#define GOM_GC_MANAGED(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_GC_MANAGED, GomGCManaged))
#define GOM_IS_GC_MANAGED(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_GC_MANAGED))
#define GOM_GC_MANAGED_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_GC_MANAGED, GomGCManagedInterface))

#define _GOM_IMPLEMENT_GC_MANAGED(i, p, f) (((GomGCManagedInterface*)i)->f = p##_##f)
#define GOM_IMPLEMENT_GC_MANAGED(i, p)                       \
    G_STMT_START {                                           \
        _GOM_IMPLEMENT_GC_MANAGED (i, p, mark_live_objects); \
    } G_STMT_END

struct _GomGCManagedInterface {
    GTypeInterface parent;

    void (*mark_live_objects) (GomGCManaged *managed, JSContext *cx, JSGCStatus status);
};

GType gom_gc_managed_get_type (void);

void  gom_gc_managed_mark_live_objects (GomGCManaged *managed,
                                        JSContext    *cx,
                                        JSGCStatus    status);

G_END_DECLS

#endif /* GOM_GC_MANAGED_H */
