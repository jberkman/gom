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

#include <gom/dom/gomcustomevent.h>

#include <time.h>

enum {
    PROP_NAMESPACE = 1,
    PROP_TYPE,
    PROP_TARGET,
    PROP_CURRENT_TARGET,
    PROP_EVENT_PHASE,
    PROP_BUBBLES,
    PROP_CANCELABLE,
    PROP_TIME_STAMP,
};

typedef struct {
    char           *namespace_uri;
    char           *type;
    GomEventTarget *target;
    GomEventTarget *current_target;
    time_t          time_stamp;
    GomPhaseType    phase_type;
    guint           bubbles             : 1;
    guint           cancelable          : 1;
    guint           stopped_propagation : 1;
    guint           stopped_immediate   : 1;
    guint           prevented_default   : 1;
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
    case PROP_NAMESPACE:
        g_value_set_string (value, priv->namespace_uri);
        break;
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
        g_value_set_enum (value, priv->phase_type);
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

static gboolean
gom_evt_is_propagation_stopped (GomCustomEvent *evt)
{
    return PRIV (evt)->stopped_propagation;
}

static void
gom_evt_stop_immediate_propagation (GomEvent *evt)
{
    PRIV (evt)->stopped_immediate = TRUE;
}

static gboolean
gom_evt_is_immediate_propagation_stopped (GomCustomEvent *evt)
{
    return PRIV (evt)->stopped_immediate;
}

static void
gom_evt_prevent_default  (GomEvent *evt)
{
    PRIV (evt)->prevented_default = TRUE;
}

static gboolean
gom_evt_is_default_prevented (GomEvent *evt)
{
    return PRIV (evt)->prevented_default;
}

static void
gom_evt_init_event_ns (GomEvent *evt,
                       const char *namespace_uri_arg,
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

    g_free (priv->namespace_uri);
    priv->namespace_uri = g_strdup (namespace_uri_arg);
}

static void
gom_evt_init_event (GomEvent   *evt,
                    const char *event_type_arg,
                    gboolean    can_bubble_arg,
                    gboolean    cancelable_arg)
{
    gom_evt_init_event_ns (evt, NULL, event_type_arg, can_bubble_arg, cancelable_arg);
}

static gboolean
gom_evt_is_custom (GomEvent *evt)
{
    return GOM_IS_CUSTOM_EVENT (evt);
}

static void
gom_evt_set_dispatch_state (GomCustomEvent *gom_custom_event,
                            GomEventTarget *target,
                            GomPhaseType    phase)
{
    GomEvtPrivate *priv = PRIV (gom_custom_event);

    GOM_SET_WEAK (priv->current_target, target);

    priv->phase_type = phase;
}

void
gom_evt_set_target (GomEvt *evt, GomEventTarget *target)
{
    GomEvtPrivate *priv = PRIV (evt);

    if (priv->target) {
        g_warning ("There is already a target set on this object.");
        return;
    }
    if (!GOM_IS_EVENT_TARGET (target)) {
        g_warning ("target is not a GomEventTarget");
        return;
    }

    GOM_SET_WEAK (priv->target, target);
}

GOM_IMPLEMENT (EVENT,        event,        gom_evt);
GOM_IMPLEMENT (CUSTOM_EVENT, custom_event, gom_evt);

G_DEFINE_TYPE_WITH_CODE (GomEvt, gom_evt, G_TYPE_OBJECT,
                         {
                             GOM_IMPLEMENT_INTERFACE (EVENT,        event,        gom_evt);
                             GOM_IMPLEMENT_INTERFACE (CUSTOM_EVENT, custom_event, gom_evt);
                         });

static void
gom_evt_init (GomEvt *event)
{
    GomEvtPrivate *priv = PRIV (event);
    
    priv->time_stamp = time (NULL);
    priv->phase_type = GOM_CAPTURING_PHASE;
}

static void
gom_evt_dispose (GObject *object)
{
    GomEvtPrivate *priv = PRIV (object);
#if 1
    g_print (G_STRLOC": %s %p\n",
             g_type_name (G_TYPE_FROM_INSTANCE (object)), object);
#endif
    g_free (priv->namespace_uri);
    priv->namespace_uri = NULL;

    g_free (priv->type);
    priv->type = NULL;

    GOM_UNSET_WEAK (priv->target);
    GOM_UNSET_WEAK (priv->current_target);

    G_OBJECT_CLASS (gom_evt_parent_class)->dispose (object);
}

static void
gom_evt_class_init (GomEvtClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (GomEvtPrivate));

    oclass->get_property = gom_evt_get_property;
    oclass->dispose      = gom_evt_dispose;

    g_object_class_override_property (oclass, PROP_TYPE,           "type");
    g_object_class_override_property (oclass, PROP_TARGET,         "target");
    g_object_class_override_property (oclass, PROP_CURRENT_TARGET, "current-target");
    g_object_class_override_property (oclass, PROP_EVENT_PHASE,    "event-phase");
    g_object_class_override_property (oclass, PROP_BUBBLES,        "bubbles");
    g_object_class_override_property (oclass, PROP_CANCELABLE,     "cancelable");
    g_object_class_override_property (oclass, PROP_TIME_STAMP,     "time-stamp");
    g_object_class_override_property (oclass, PROP_NAMESPACE,      "namespace-u-r-i");
}
