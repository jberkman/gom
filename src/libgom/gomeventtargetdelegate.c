/*The MIT License

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

#include "gom/gomeventtargetdelegate.h"

#include "gommacros.h"

enum {
    PROP_TARGET = 1
};

typedef struct {
    GomEventTarget *target;
} GomEventTargetDelegatePrivate;

#define PRIV(i) G_TYPE_INSTANCE_GET_PRIVATE ((i), GOM_TYPE_EVENT_TARGET_DELEGATE, GomEventTargetDelegatePrivate)

static void
gom_event_target_delegate_set_property (GObject      *object,
                                        guint         property_id,
                                        const GValue *value,
                                        GParamSpec   *pspec)
{
    GomEventTargetDelegatePrivate *priv = PRIV (object);

    switch (property_id) {
    case PROP_TARGET:
        if (priv->target) {
            g_warning ("%s:%d:%s(): target already set on %p\n",
                       __FILE__, __LINE__, __FUNCTION__, object);
            return;
        }
        priv->target = g_value_get_object (value);
        if (!priv->target) {
            g_warning ("%s:%d:%s(): target for %p should not be NULL\n",
                       __FILE__, __LINE__, __FUNCTION__, object);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
gom_event_target_delegate_add_event_listener (GomEventTarget   *target,
                                              const char       *type,
                                              GomEventListener *listener,
                                              gboolean          use_capture)
{
    GOM_NOT_IMPLEMENTED;
}

static void
gom_event_target_delegate_remove_event_listener (GomEventTarget   *target,
                                                 const char       *type,
                                                 GomEventListener *listener,
                                                 gboolean          use_capture)
{
    GOM_NOT_IMPLEMENTED;
}

static gboolean
gom_event_target_delegate_dispatch_event (GomEventTarget *target,
                                          GomEvent       *evt,
                                          GError        **error)
{
    GomEventTargetDelegatePrivate *priv = PRIV (target);

    char *event_name;
    g_object_get (evt, "type", &event_name, NULL);
    g_print (" &&& dispatching event: %s.%s\n", g_type_name (G_TYPE_FROM_INSTANCE (priv->target)), event_name);
    g_free (event_name);
    return FALSE;
}

static void
gom_event_target_delegate_impl_gom_event_target (gpointer g_iface, gpointer iface_data)
{
    GomEventTargetInterface *iface = (GomEventTargetInterface *)g_iface;

#define IFACE(func) iface->func = gom_event_target_delegate_##func

    IFACE (add_event_listener);
    IFACE (remove_event_listener);
    IFACE (dispatch_event);

#undef IFACE
}

G_DEFINE_TYPE_WITH_CODE (GomEventTargetDelegate, gom_event_target_delegate, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (GOM_TYPE_EVENT_TARGET, 
                                                gom_event_target_delegate_impl_gom_event_target));

static void gom_event_target_delegate_init (GomEventTargetDelegate *del) { }

static void
gom_event_target_delegate_class_init (GomEventTargetDelegateClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (GomEventTargetDelegatePrivate));

    oclass->set_property = gom_event_target_delegate_set_property;

    g_object_class_install_property (
        oclass, PROP_TARGET,
        g_param_spec_object ("target", NULL,
                             "The EventTarget of which this is a delegate.",
                             GOM_TYPE_EVENT_TARGET,
                             G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));
}

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE(GomEventTargetHelper, gom_event_target_helper,
                                       GOM_NOT_IMPLEMENTED, GOM_TYPE_EVENT_TARGET);

GOM_STUB_FUNC (GOM_EVENT_TARGET_HELPER, gom_event_target_helper, get_event_target_delegate,
               (GomEventTargetHelper *gom_event_target_helper), (gom_event_target_helper),
               GomEventTarget *);
