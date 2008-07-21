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

#include "gom/dom/gommouseevent.h"

#include "gom/gommouseevt.h"

#include "gom/dom/gomuievent.h"

#include "gommacros.h"

#include <gdk/gdktypes.h>

enum {
    PROP_SCREEN_X = 1,
    PROP_SCREEN_Y,
    PROP_CLIENT_X,
    PROP_CLIENT_Y,
    PROP_CTRL_KEY,
    PROP_SHIFT_KEY,
    PROP_ALT_KEY,
    PROP_META_KEY,
    PROP_BUTTON,
    PROP_RELATED_TARGET
};

typedef struct {
    GomEventTarget *related_target;
    long screen_x;
    long screen_y;
    long client_x;
    long client_y;
    guint button;
    guint state;
} GomMouseEvtPrivate;

#define PRIV(i) G_TYPE_INSTANCE_GET_PRIVATE ((i), GOM_TYPE_MOUSE_EVT, GomMouseEvtPrivate)

static void
gom_mouse_evt_get_property (GObject    *object,
                            guint       property_id,
                            GValue     *value,
                            GParamSpec *pspec)
{
    GomMouseEvtPrivate *priv = PRIV (object);

    switch (property_id) {
    case PROP_SCREEN_X:
        g_value_set_long (value, priv->screen_x);
        break;
    case PROP_SCREEN_Y:
        g_value_set_long (value, priv->screen_y);
        break;
    case PROP_CLIENT_X:
        g_value_set_long (value, priv->client_x);
        break;
    case PROP_CLIENT_Y:
        g_value_set_long (value, priv->client_y);
        break;
    case PROP_CTRL_KEY:
        g_value_set_boolean (value, priv->state & GDK_CONTROL_MASK);
        break;
    case PROP_SHIFT_KEY:
        g_value_set_boolean (value, priv->state & GDK_SHIFT_MASK);
        break;
    case PROP_ALT_KEY:
        g_value_set_boolean (value, priv->state & GDK_MOD1_MASK);
        break;
    case PROP_META_KEY:
        g_value_set_boolean (value, priv->state & GDK_META_MASK);
        break;
    case PROP_BUTTON:
        g_value_set_uint (value, priv->button);
        break;
    case PROP_RELATED_TARGET:
        g_value_set_object (value, priv->related_target);
        break;
    default: 
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
gom_mouse_evt_init_mouse_event_ns (GomMouseEvent   *evt,
                                   const char      *namespace_uri,
                                   const char      *event_type_arg,
                                   gboolean         can_bubble_arg,
                                   gboolean         cancelable_arg,
                                   GomAbstractView *view_arg,
                                   long             detail_arg,
                                   long             screen_x_arg,
                                   long             screen_y_arg,
                                   long             client_x_arg,
                                   long             client_y_arg,
                                   gboolean         ctrl_key_arg,
                                   gboolean         alt_key_arg,
                                   gboolean         shift_key_arg,
                                   gboolean         meta_key_arg,
                                   guint            button_arg,
                                   GomEventTarget  *related_target_arg)
{
    GomMouseEvtPrivate *priv = PRIV (evt);
    GomEventTarget *target;

    g_object_get (evt, "target", &target, NULL);
    if (target) {
        g_warning ("Event %p has already begun dispatching; changes are not permitted.", evt);
        g_object_unref (target);
        return;
    }

    gom_ui_event_init_ui_event_ns (GOM_UI_EVENT (evt), namespace_uri, event_type_arg,
                                   can_bubble_arg, cancelable_arg, view_arg, detail_arg);

    priv->screen_x  = screen_x_arg;
    priv->screen_y  = screen_y_arg;
    priv->client_x  = client_x_arg;
    priv->client_y  = client_y_arg;
    priv->button    = button_arg;

    priv->state = 0;
    if (ctrl_key_arg)  priv->state |= GDK_CONTROL_MASK;
    if (shift_key_arg) priv->state |= GDK_SHIFT_MASK;
    if (alt_key_arg)   priv->state |= GDK_MOD1_MASK;
    if (meta_key_arg)  priv->state |= GDK_META_MASK;

    if (priv->related_target) {
        g_object_unref (priv->related_target);
    }
    priv->related_target = related_target_arg ? g_object_ref (related_target_arg) : NULL;
}

static void
gom_mouse_evt_init_mouse_event (GomMouseEvent   *evt,
                                const char      *event_type_arg,
                                gboolean         can_bubble_arg,
                                gboolean         cancelable_arg,
                                GomAbstractView *view_arg,
                                long             detail_arg,
                                long             screen_x_arg,
                                long             screen_y_arg,
                                long             client_x_arg,
                                long             client_y_arg,
                                gboolean         ctrl_key_arg,
                                gboolean         alt_key_arg,
                                gboolean         shift_key_arg,
                                gboolean         meta_key_arg,
                                guint            button_arg,
                                GomEventTarget  *related_target_arg)
{
    gom_mouse_evt_init_mouse_event_ns (evt, NULL, event_type_arg, can_bubble_arg, cancelable_arg, view_arg, detail_arg, screen_x_arg, screen_y_arg, client_x_arg, client_y_arg, ctrl_key_arg, alt_key_arg, shift_key_arg, meta_key_arg, button_arg, related_target_arg);
}

static gboolean
gom_mouse_evt_get_modifier_state (GomMouseEvent *evt,
                                  const char    *modifiers_list)
{
    GOM_NOT_IMPLEMENTED;
    return FALSE;
}

GOM_IMPLEMENT (MOUSE_EVENT, mouse_event, gom_mouse_evt);

G_DEFINE_TYPE_WITH_CODE (GomMouseEvt, gom_mouse_evt, GOM_TYPE_UI_EVT,
                         GOM_IMPLEMENT_INTERFACE (MOUSE_EVENT, mouse_event, gom_mouse_evt));

static void gom_mouse_evt_init (GomMouseEvt *event) { }

static void
gom_mouse_evt_class_init (GomMouseEvtClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (GomMouseEvtPrivate));

    oclass->get_property = gom_mouse_evt_get_property;

    g_object_class_override_property (oclass, PROP_SCREEN_X,       "screen-x");
    g_object_class_override_property (oclass, PROP_SCREEN_Y,       "screen-y");
    g_object_class_override_property (oclass, PROP_CLIENT_X,       "client-x");
    g_object_class_override_property (oclass, PROP_CLIENT_Y,       "client-y");
    g_object_class_override_property (oclass, PROP_CTRL_KEY,       "ctrl-key");
    g_object_class_override_property (oclass, PROP_SHIFT_KEY,      "shift-key");
    g_object_class_override_property (oclass, PROP_ALT_KEY,        "alt-key");
    g_object_class_override_property (oclass, PROP_META_KEY,       "meta-key");
    g_object_class_override_property (oclass, PROP_BUTTON,         "button");
    g_object_class_override_property (oclass, PROP_RELATED_TARGET, "related-target");
}
