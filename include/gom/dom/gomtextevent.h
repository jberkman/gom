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
#ifndef GOM_TEXT_EVENT_H
#define GOM_TEXT_EVENT_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomTextEvent          GomTextEvent; /*dummy object*/
typedef struct _GomTextEventInterface GomTextEventInterface;

G_END_DECLS

#include <gom/dom/gomabstractview.h>
#include <gom/dom/gomeventtarget.h>

G_BEGIN_DECLS

#define GOM_TYPE_TEXT_EVENT             (gom_text_event_get_type ())
#define GOM_TEXT_EVENT(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_TEXT_EVENT, GomTextEvent))
#define GOM_IS_TEXT_EVENT(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_TEXT_EVENT))
#define GOM_TEXT_EVENT_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_TEXT_EVENT, GomTextEventInterface))

#define _GOM_IMPLEMENT_TEXT_EVENT(i, p, f) (((GomTextEventInterface*)i)->f = p##_##f)
#define GOM_IMPLEMENT_TEXT_EVENT(i, p)                          \
    G_STMT_START {                                              \
        _GOM_IMPLEMENT_TEXT_EVENT (i, p, init_text_event);      \
        _GOM_IMPLEMENT_TEXT_EVENT (i, p, init_text_event_ns);   \
    } G_STMT_END

struct _GomTextEventInterface {
    GTypeInterface parent;

    void (*init_text_event) (GomTextEvent    *gom_text_event,
                             const char      *type_arg,
                             gboolean         can_bubble_arg,
                             gboolean         cancelable_arg,
                             GomAbstractView *view_arg,
                             const char      *data_arg);

    void (*init_text_event_ns) (GomTextEvent    *gom_text_event,
                                const char      *namespace_uri,
                                const char      *type_arg,
                                gboolean         can_bubble_arg,
                                gboolean         cancelable_arg,
                                GomAbstractView *view_arg,
                                const char      *data_arg);
};

GType gom_text_event_get_type (void);

void  gom_text_event_init_text_event (GomTextEvent    *gom_text_event,
                                      const char      *type_arg,
                                      gboolean         can_bubble_arg,
                                      gboolean         cancelable_arg,
                                      GomAbstractView *view_arg,
                                      const char      *data_arg);

void  gom_text_event_init_text_event_ns (GomTextEvent    *gom_text_event,
                                         const char      *namespace_uri,
                                         const char      *type_arg,
                                         gboolean         can_bubble_arg,
                                         gboolean         cancelable_arg,
                                         GomAbstractView *view_arg,
                                         const char      *data_arg);

G_END_DECLS

#endif /* GOM_TEXT_EVENT_H */
