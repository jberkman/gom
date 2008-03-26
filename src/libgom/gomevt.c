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

#include <gom/gomevt.h>

#include "gommacros.h"

#include <time.h>

enum {
    PROP_TYPE = 1,
    PROP_TARGET,
    PROP_CURRENT_TARGET,
    PROP_EVENT_PHASE,
    PROP_BUBBLES,
    PROP_CANCELABLE,
    PROP_TIME_STAMP
};

typedef struct {
    char              *type;
    GomEventTarget    *target;
    GomEventTarget    *current_target;
    time_t             time_stamp;
    GomEventPhaseType  phase_type;
    guint              bubbles             : 1;
    guint              cancelable          : 1;
    guint              stopped_propagation : 1;
    guint              prevented_default   : 1;
} GomEvtPrivate;

#define PRIV(i) G_TYPE_INSTANCE_GET_PRIVATE ((i), GOM_TYPE_EVT, GomEvtPrivate)

static void
gom_evt_get_property (GObject    *object,
                      guint       property_id,
                      GValue     *value,
                      GParamSpec *pspec)
{
    GomEvtPrivate *priv = PRIV (object);

    switch (property_id) {
    case PROP_TYPE:
        g_value_set_string (value, priv->type);
        break;
    case PROP_TARGET:
        g_value_set_object (value, priv->target);
        break;
    case PROP_CURRENT_TARGET:
        g_value_set_object (value, priv->current_target);
        break;
    case PROP_EVENT_PHASE:
        g_value_set_int (value, priv->phase_type);
        break;
    case PROP_BUBBLES:
        g_value_set_boolean (value, priv->bubbles);
        break;
    case PROP_CANCELABLE:
        g_value_set_boolean (value, priv->cancelable);
        break;
    case PROP_TIME_STAMP:
        g_value_set_long (value, priv->time_stamp);
        break;
    default: 
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
gom_evt_stop_propagation (GomEvent *evt)
{
    PRIV (evt)->stopped_propagation = TRUE;
}

gboolean
gom_evt_stopped_propagation (GomEvt *evt)
{
    return PRIV (evt)->stopped_propagation;
}

static void
gom_evt_prevent_default  (GomEvent *evt)
{
    PRIV (evt)->prevented_default = TRUE;
}

gboolean
gom_evt_prevented_default (GomEvt *evt)
{
    return PRIV (evt)->prevented_default;
}

static void
gom_evt_init_event (GomEvent   *evt,
                    const char *event_type_arg,
                    gboolean    can_bubble_arg,
                    gboolean    cancelable_arg)
{
    GomEvtPrivate *priv = PRIV (evt);

    if (priv->target) {
        g_warning ("Event %p has already begun dispatching; changes are not permitted.", evt);
        return;
    }

    priv->bubbles    = can_bubble_arg;
    priv->cancelable = cancelable_arg;

    g_free (priv->type);
    priv->type = g_strdup (event_type_arg);
}

static void
gom_evt_impl_gom_event (gpointer g_iface, gpointer iface_data)
{
    GomEventInterface *iface = (GomEventInterface *)g_iface;

#define IFACE(func) iface->func = gom_evt_##func

    IFACE (stop_propagation);
    IFACE (prevent_default);
    IFACE (init_event);
    
#undef IFACE
}

G_DEFINE_TYPE_WITH_CODE (GomEvt, gom_evt, G_TYPE_OBJECT,
                         {
                             G_IMPLEMENT_INTERFACE (GOM_TYPE_EVENT, gom_evt_impl_gom_event);
                         });

static void
gom_evt_init (GomEvt *event)
{
    GomEvtPrivate *priv = PRIV (event);
    
    priv->time_stamp = time (NULL);
    priv->phase_type = GOM_EVENT_CAPTURING_PHASE;
}

static void
gom_evt_class_init (GomEvtClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (GomEvtPrivate));

    oclass->get_property = gom_evt_get_property;

    g_object_class_override_property (oclass, PROP_TYPE,           "type");
    g_object_class_override_property (oclass, PROP_TARGET,         "target");
    g_object_class_override_property (oclass, PROP_CURRENT_TARGET, "current-target");
    g_object_class_override_property (oclass, PROP_EVENT_PHASE,    "event-phase");
    g_object_class_override_property (oclass, PROP_BUBBLES,        "bubbles");
    g_object_class_override_property (oclass, PROP_CANCELABLE,     "cancelable");
    g_object_class_override_property (oclass, PROP_TIME_STAMP,     "time-stamp");
}
