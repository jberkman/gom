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
#ifndef GOM_EVENT_TARGET_H
#define GOM_EVENT_TARGET_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomEventTarget          GomEventTarget; /* dummy object */
typedef struct _GomEventTargetInterface GomEventTargetInterface;

G_END_DECLS

#include <gom/dom/gomeventlistener.h>

G_BEGIN_DECLS

#define GOM_TYPE_EVENT_TARGET             (gom_event_target_get_type ())
#define GOM_EVENT_TARGET(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_EVENT_TARGET, GomEventTarget))
#define GOM_IS_EVENT_TARGET(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_EVENT_TARGET))
#define GOM_EVENT_TARGET_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_EVENT_TARGET, GomEventTargetInterface))

#define _GOM_IMPLEMENT_EVENT_TARGET(i, p, f) (((GomEventTargetInterface*)i)->f = p##_##f)
#define GOM_IMPLEMENT_EVENT_TARGET(i, p)                                \
    G_STMT_START {                                                      \
        _GOM_IMPLEMENT_EVENT_TARGET(i, p, add_event_listener);          \
        _GOM_IMPLEMENT_EVENT_TARGET(i, p, remove_event_listener);       \
        _GOM_IMPLEMENT_EVENT_TARGET(i, p, dispatch_event);              \
        _GOM_IMPLEMENT_EVENT_TARGET(i, p, add_event_listener_ns);       \
        _GOM_IMPLEMENT_EVENT_TARGET(i, p, remove_event_listener_ns);    \
        _GOM_IMPLEMENT_EVENT_TARGET(i, p, will_trigger_ns);             \
        _GOM_IMPLEMENT_EVENT_TARGET(i, p, has_event_listener_ns);       \
    } G_STMT_END

struct _GomEventTargetInterface {
    GTypeInterface parent;

    void     (*add_event_listener)    (GomEventTarget   *target,
                                       const char       *type,
                                       GomEventListener *listener,
                                       gboolean          use_capture);

    void     (*remove_event_listener) (GomEventTarget   *target,
                                       const char       *type,
                                       GomEventListener *listener,
                                       gboolean          use_capture);

    gboolean (*dispatch_event)        (GomEventTarget   *target,
                                       GomEvent         *evt,
                                       GError          **error);

    void     (*add_event_listener_ns)    (GomEventTarget   *event_target,
                                          const char       *namespace_uri,
                                          const char       *type,
                                          GomEventListener *listener,
                                          gboolean          use_capture,
                                          GObject          *evt_group);

    void     (*remove_event_listener_ns) (GomEventTarget   *target,
                                          const char       *namespace_uri,
                                          const char       *type,
                                          GomEventListener *listener,
                                          gboolean          use_capture);

    gboolean (*will_trigger_ns)          (GomEventTarget   *target,
                                          const char       *namespace_uri,
                                          const char       *type);

    gboolean (*has_event_listener_ns)    (GomEventTarget   *target,
                                          const char       *namespace_uri,
                                          const char       *type);
};

GType gom_event_target_get_type (void);

void     gom_event_target_add_event_listener    (GomEventTarget *target,
                                                 const char       *type,
                                                 GomEventListener *listener,
                                                 gboolean          use_capture);

void     gom_event_target_remove_event_listener (GomEventTarget   *target,
                                                 const char       *type,
                                                 GomEventListener *listener,
                                                 gboolean          use_capture);

gboolean gom_event_target_dispatch_event        (GomEventTarget   *target,
                                                 GomEvent         *evt,
                                                 GError          **error);

void     gom_event_target_add_event_listener_ns    (GomEventTarget   *event_target,
                                                    const char       *namespace_uri,
                                                    const char       *type,
                                                    GomEventListener *listener,
                                                    gboolean          use_capture,
                                                    GObject          *evt_group);

void     gom_event_target_remove_event_listener_ns (GomEventTarget   *target,
                                                    const char       *namespace_uri,
                                                    const char       *type,
                                                    GomEventListener *listener,
                                                    gboolean          use_capture);

gboolean gom_event_target_will_trigger_ns          (GomEventTarget   *target,
                                                    const char       *namespace_uri,
                                                    const char       *type);

gboolean gom_event_target_has_event_listener_ns    (GomEventTarget   *target,
                                                    const char       *namespace_uri,
                                                    const char       *type);

G_END_DECLS

#endif /* GOM_EVENT_TARGET_H */
