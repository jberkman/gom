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
#ifndef GOM_EVENT_H
#define GOM_EVENT_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomEvent          GomEvent; /* dummy object */
typedef struct _GomEventInterface GomEventInterface;

typedef enum {
    GOM_CAPTURING_PHASE = 1,
    GOM_AT_TARGET,
    GOM_BUBBLING_PHASE
} GomPhaseType;

G_END_DECLS

#include <glib-object.h>

G_BEGIN_DECLS

#define GOM_TYPE_EVENT             (gom_event_get_type ())
#define GOM_EVENT(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_EVENT, GomEvent))
#define GOM_IS_EVENT(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_EVENT))
#define GOM_EVENT_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_EVENT, GomEventInterface))

struct _GomEventInterface {
    GTypeInterface parent;

    void (*stop_propagation) (GomEvent *evt);
    void (*prevent_default)  (GomEvent *evt);
    void (*init_event)       (GomEvent *evt,
                              const char *event_type_arg,
                              gboolean    can_bubble_arg,
                              gboolean    cancelable_arg);

    gboolean (*is_custom)                  (GomEvent *evt);
    void     (*stop_immediate_propagation) (GomEvent *evt);
    gboolean (*is_default_prevented)       (GomEvent *evt);
    void     (*init_event_ns)              (GomEvent *evt,
                                            const char *namespace_uri_arg,
                                            const char *event_type_arg,
                                            gboolean    can_bubble_arg,
                                            gboolean    cancelable_arg);
};

GType gom_event_get_type (void);

void gom_event_stop_propagation (GomEvent *evt);
void gom_event_prevent_default  (GomEvent *evt);
void gom_event_init_event       (GomEvent *evt,
                                 const char *event_type_arg,
                                 gboolean    can_bubble_arg,
                                 gboolean    cancelable_arg);

gboolean gom_event_is_custom                  (GomEvent *evt);
void     gom_event_stop_immediate_propagation (GomEvent *evt);
gboolean gom_event_is_default_prevented       (GomEvent *evt);
void     gom_event_init_event_ns              (GomEvent *evt,
                                               const char *namespace_uri_arg,
                                               const char *event_type_arg,
                                               gboolean    can_bubble_arg,
                                               gboolean    cancelable_arg);
G_END_DECLS

#endif /* GOM_EVENT_H */
