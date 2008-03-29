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
#ifndef GOM_CUSTOM_EVENT_H
#define GOM_CUSTOM_EVENT_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomCustomEvent          GomCustomEvent; /* dummy object */
typedef struct _GomCustomEventInterface GomCustomEventInterface;

G_END_DECLS

#include <gom/dom/gomeventtarget.h>

G_BEGIN_DECLS

#define GOM_TYPE_CUSTOM_EVENT             (gom_custom_event_get_type ())
#define GOM_CUSTOM_EVENT(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_CUSTOM_EVENT, GomCustomEvent))
#define GOM_IS_CUSTOM_EVENT(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_CUSTOM_EVENT))
#define GOM_CUSTOM_EVENT_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_CUSTOM_EVENT, GomCustomEventInterface))

#define _GOM_IMPLEMENT_CUSTOM_EVENT(i, p, f) (((GomCustomEventInterface*)i)->f = p##_##f)
#define GOM_IMPLEMENT_CUSTOM_EVENT(i, p)                                \
    G_STMT_START {                                                      \
        _GOM_IMPLEMENT_CUSTOM_EVENT (i, p, set_dispatch_state);         \
        _GOM_IMPLEMENT_CUSTOM_EVENT (i, p, is_propagation_stopped);     \
        _GOM_IMPLEMENT_CUSTOM_EVENT (i, p, is_immediate_propagation_stopped); \
    } G_STMT_END

struct _GomCustomEventInterface {
    GTypeInterface parent;

    void     (*set_dispatch_state)               (GomCustomEvent *gom_custom_event,
                                                  GomEventTarget *target,
                                                  GomPhaseType    phase);

    gboolean (*is_propagation_stopped)           (GomCustomEvent *gom_custom_event);

    gboolean (*is_immediate_propagation_stopped) (GomCustomEvent *gom_custom_event);
};

GType    gom_custom_event_get_type (void);

void     gom_custom_event_set_dispatch_state               (GomCustomEvent *gom_custom_event,
                                                            GomEventTarget *target,
                                                            GomPhaseType    phase);

gboolean gom_custom_event_is_propagation_stopped           (GomCustomEvent *gom_custom_event);

gboolean gom_custom_event_is_immediate_propagation_stopped (GomCustomEvent *gom_custom_event);

G_END_DECLS

#endif /* GOM_CUSTOM_EVENT_H */
