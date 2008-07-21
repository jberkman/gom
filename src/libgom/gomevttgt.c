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
#include "config.h"

#include "gom/gomeventtargetinternal.h"

#include "gom/gomevttgt.h"

#include "gom/gomlistenerlist.h"

#include "gommacros.h"

typedef struct {
    GomListenerList *listeners;
} GomEvtTgtPrivate;

#define PRIV(i) G_TYPE_INSTANCE_GET_PRIVATE ((i), GOM_TYPE_EVT_TGT, GomEvtTgtPrivate)

static void
gom_evt_tgt_add_event_listener_ns (GomEventTarget   *event_target,
                                   const char       *namespace_uri,
                                   const char       *type,
                                   GomEventListener *listener,
                                   gboolean          use_capture,
                                   GObject          *evt_group)
{
    gom_listener_list_add_event_listener (
        PRIV (event_target)->listeners, namespace_uri,
        type, listener, use_capture, evt_group);
}

static void
gom_evt_tgt_add_event_listener (GomEventTarget   *event_target,
                                const char       *type,
                                GomEventListener *listener,
                                gboolean          use_capture)
{
    gom_listener_list_add_event_listener (
        PRIV (event_target)->listeners, NULL,
        type, listener, use_capture, NULL);
}

static void
gom_evt_tgt_remove_event_listener_ns (GomEventTarget   *target,
                                      const char       *namespace_uri,
                                      const char       *type,
                                      GomEventListener *listener,
                                      gboolean          use_capture)
{
    gom_listener_list_remove_event_listener (
        PRIV (target)->listeners, namespace_uri,
        type, listener, use_capture);
}

static void
gom_evt_tgt_remove_event_listener (GomEventTarget   *target,
                                   const char       *type,
                                   GomEventListener *listener,
                                   gboolean          use_capture)
{
    gom_listener_list_remove_event_listener (
        PRIV (target)->listeners, NULL,
        type, listener, use_capture);
}

static gboolean
gom_evt_tgt_has_event_listener_ns (GomEventTarget   *target,
                                   const char       *namespace_uri,
                                   const char       *type)
{
    return gom_listener_list_has_event_listener (PRIV (target)->listeners, namespace_uri, type);
}

static gboolean
gom_evt_tgt_will_trigger_ns (GomEventTarget   *target,
                             const char       *namespace_uri,
                             const char       *type)
{
    return gom_listener_list_will_trigger (PRIV (target)->listeners, target, namespace_uri, type);
}

#define gom_evt_tgt_dispatch_event gom_listener_list_dispatch_event

void
gom_evt_tgt_dispatch_listeners (GomEventTargetInternal *current_target,
                                GomEvent               *evt,
                                const char             *namespace_uri,
                                const char             *type_name,
                                GomPhaseType            phase)
{
    gom_listener_list_dispatch_listeners (
        PRIV (current_target)->listeners,
        GOM_EVENT_TARGET (current_target),
        evt, namespace_uri, type_name, phase);
}

GOM_IMPLEMENT (EVENT_TARGET, event_target, gom_evt_tgt);

G_DEFINE_TYPE_WITH_CODE (GomEvtTgt, gom_evt_tgt, G_TYPE_OBJECT,
                         GOM_IMPLEMENT_INTERFACE (EVENT_TARGET, event_target, gom_evt_tgt));

static void
gom_evt_tgt_init (GomEvtTgt *event)
{
    GomEvtTgtPrivate *priv = PRIV (event);

    priv->listeners = gom_listener_list_new ();
}

static void
gom_evt_tgt_dispose (GObject *object)
{
    GomEvtTgtPrivate *priv = PRIV (object);

    gom_listener_list_free (priv->listeners);
    priv->listeners = NULL;

    G_OBJECT_CLASS (gom_evt_tgt_parent_class)->dispose (object);
}

static void
gom_evt_tgt_class_init (GomEvtTgtClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (GomEvtTgtPrivate));

    oclass->dispose = gom_evt_tgt_dispose;
}

