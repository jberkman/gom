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
#ifndef GOM_UNKNOWN_H
#define GOM_UNKNOWN_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomUnknown GomUnknown; /* dummy object */
typedef struct _GomUnknownInterface GomUnknownInterface;

G_END_DECLS

#include <glib-object.h>

G_BEGIN_DECLS

#define GOM_TYPE_UNKNOWN             (gom_unknown_get_type ())
#define GOM_UNKNOWN(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_UNKNOWN, GomUnknown))
#define GOM_IS_UNKNOWN(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_UNKNOWN))
#define GOM_UNKNOWN_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_UNKNOWN, GomUnknownInterface))

#define _GOM_IMPLEMENT_UNKNOWN(i, p, f) (((GomUnknownInterface*)i)->f = p##_##f)
#define GOM_IMPLEMENT_UNKNOWN(i, p)                        \
    G_STMT_START {                                         \
        _GOM_IMPLEMENT_UNKNOWN (i, p, query_interface);    \
    } G_STMT_END

struct _GomUnknownInterface {
    GTypeInterface parent;
    gpointer (*query_interface) (gpointer unknown,
                                 GType requested_interface,
                                 GError **error);
};

GType gom_unknown_get_type (void);

gpointer gom_unknown_query_interface (gpointer unknown,
                                      GType requested_interface,
                                      GError **error);

G_END_DECLS

#endif /* GOM_UNKNOWN_H */
