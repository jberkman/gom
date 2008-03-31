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
#ifndef GOM_TARGET_H
#define GOM_TARGET_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomTargetProxy          GomTargetProxy; /* dummy object */
typedef struct _GomTargetProxyInterface GomTargetProxyInterface;

typedef struct _GomTarget      GomTarget;
typedef struct _GomTargetClass GomTargetClass;

G_END_DECLS

#include <glib-object.h>

G_BEGIN_DECLS

#define GOM_TYPE_TARGET_PROXY             (gom_target_proxy_get_type ())
#define GOM_TARGET_PROXY(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_TARGET_PROXY, GomTargetProxy))
#define GOM_IS_TARGET_PROXY(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_TARGET_PROXY))
#define GOM_TARGET_PROXY_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_TARGET_PROXY, GomTargetProxyInterface))

#define _GOM_IMPLEMENT_TARGET_PROXY(i, p, f) (((GomTargetProxyInterface*)i)->f = p##_##f)
#define GOM_IMPLEMENT_TARGET_PROXY(i, p)                         \
    G_STMT_START {                                                      \
        _GOM_IMPLEMENT_TARGET_PROXY (i, p, get_target); \
    } G_STMT_END

#define GOM_TYPE_TARGET           (gom_target_get_type ())
#define GOM_TARGET(i)             (G_TYPE_CHECK_INSTANCE_CAST ((i), GOM_TYPE_TARGET, GomTarget))
#define GOM_TARGET_CLASS(k)       (G_TYPE_CHECK_CLASS_CAST    ((k), GOM_TYPE_TARGET, GomTargetClass))
#define GOM_IS_TARGET(i)          (G_TYPE_CHECK_INSTANCE_TYPE ((i), GOM_TYPE_TARGET))
#define GOM_IS_TARGET_CLASS(k)    (G_TYPE_CHECK_CLASS_TYPE    ((k), GOM_TYPE_TARGET))
#define GOM_TARGET_GET_CLASS(i)   (G_TYPE_INSTANCE_GET_CLASS  ((i), GOM_TYPE_TARGET, GomTargetClass))

struct _GomTargetProxyInterface {
    GTypeInterface parent;
    GomTarget *(*get_target) (GomTargetProxy *gom_target_proxy);
};

struct _GomTarget {
    GObject parent;
};

struct _GomTargetClass {
    GObjectClass parent_class;
};

GType      gom_target_proxy_get_type   (void);
GType      gom_target_get_type         (void);

GomTarget *gom_target_proxy_get_target (GomTargetProxy *gom_target_proxy);

G_END_DECLS

#endif /* GOM_TARGET_H */
