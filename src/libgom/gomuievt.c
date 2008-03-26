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

#include <gom/gomuievt.h>

#include "gommacros.h"

#include <gom/dom/gomabstractview.h>
#include <gom/dom/gomuievent.h>

enum {
    PROP_VIEW = 1,
    PROP_DETAIL
};

typedef struct {
    GomAbstractView *view;
    long             detail;
} GomUIEvtPrivate;

#define PRIV(i) G_TYPE_INSTANCE_GET_PRIVATE ((i), GOM_TYPE_UI_EVT, GomUIEvtPrivate)

static void
gom_ui_evt_get_property (GObject    *object,
                      guint       property_id,
                      GValue     *value,
                      GParamSpec *pspec)
{
    GomUIEvtPrivate *priv = PRIV (object);

    switch (property_id) {
    case PROP_VIEW:
        g_value_set_object (value, priv->view);
        break;
    case PROP_DETAIL:
        g_value_set_long (value, priv->detail);
        break;
    default: 
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
gom_ui_evt_init_ui_event (GomUIEvent      *evt,
                          const char      *event_type_arg,
                          gboolean         can_bubble_arg,
                          gboolean         cancelable_arg,
                          GomAbstractView *view_arg,
                          long             detail_arg)
{
    GomUIEvtPrivate *priv = PRIV (evt);
    GomEventTarget *target;

    g_object_get (evt, "target", &target, NULL);
    if (target) {
        g_warning ("Event %p has already begun dispatching; changes are not permitted.", evt);
        return;
    }

    gom_event_init_event (GOM_EVENT (evt), event_type_arg, can_bubble_arg, cancelable_arg);

    if (priv->view) {
        g_object_unref (priv->view);
    }
    priv->view = view_arg ? g_object_ref (view_arg) : NULL;

    priv->detail = detail_arg;
}

static void
gom_ui_evt_impl_gom_ui_event (gpointer g_iface, gpointer iface_data)
{
    GomUIEventInterface *iface = (GomUIEventInterface *)g_iface;

#define IFACE(func) iface->func = gom_ui_evt_##func

    IFACE (init_ui_event);
    
#undef IFACE
}

G_DEFINE_TYPE_WITH_CODE (GomUIEvt, gom_ui_evt, GOM_TYPE_EVT,
                         {
                             G_IMPLEMENT_INTERFACE (GOM_TYPE_UI_EVENT, gom_ui_evt_impl_gom_ui_event);
                         });

static void gom_ui_evt_init (GomUIEvt *event) { }

static void
gom_ui_evt_class_init (GomUIEvtClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (GomUIEvtPrivate));

    oclass->get_property = gom_ui_evt_get_property;

    g_object_class_override_property (oclass, PROP_VIEW,   "view");
    g_object_class_override_property (oclass, PROP_DETAIL, "detail");
}
