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

#include <gom/gomwidget.h>

#include <gommacros.h>

#include <gom/dom/gomdomexception.h>
#include <gom/dom/gomelement.h>
#include <gom/dom/gomeventtarget.h>
#include <gom/dom/gomkeyboardevent.h>
#include <gom/dom/gommouseevent.h>
#include <gom/dom/gomuievent.h>
#include <gom/gomglist.h>
#include <gom/gomjselement.h>
#include <gom/gomjsobject.h>
#include <gom/gomkeyboardevt.h>
#include <gom/gommouseevt.h>
#include <gom/gomobject.h>

#include <gtk/gtk.h>

#include <strings.h>
#include <stdlib.h>
#include <limits.h>

enum {
    PROP_NODE_NAME = 1078,
    PROP_NODE_VALUE,
    PROP_NODE_TYPE,
    PROP_PARENT_NODE,
    PROP_CHILD_NODES,
    PROP_FIRST_CHILD,
    PROP_LAST_CHILD,
    PROP_PREVIOUS_SIBLING,
    PROP_NEXT_SIBLING,
    PROP_ATTRIBUTES,
    PROP_OWNER_DOCUMENT,
    PROP_NAMESPACE_URI,
    PROP_PREFIX,
    PROP_LOCAL_NAME,

    PROP_TAG_NAME
};

GQuark gom_widget_private_quark (void);
GOM_DEFINE_QUARK (widget_private);

typedef struct {
    guint click_state;
} GomWidgetPrivate;

static void
free_priv (gpointer data)
{
    GomWidgetPrivate *priv = data;

    g_free (priv);
}

static GomWidgetPrivate *
get_priv (gpointer obj)
{
    GomWidgetPrivate *priv;

    priv = g_object_get_qdata (obj, gom_widget_private_quark ());
    if (!priv) {
        priv = g_new0 (GomWidgetPrivate, 1);
        g_object_set_qdata_full (obj, gom_widget_private_quark (), priv, free_priv);
    }
    return priv;
}

static void (*_gtk_widget_set_property) (GObject        *object,
                                         guint           property_id,
                                         const GValue   *value,
                                         GParamSpec     *pspec);
static void (*_gtk_widget_get_property) (GObject        *object,
                                         guint           property_id,
                                         GValue         *value,
                                         GParamSpec     *pspec);

static gboolean (*_gtk_widget_event)    (GtkWidget      *widget,
                                         GdkEvent       *event);

/* attributes */

static GomNode *
widget_get_first_child (GomNode *node)
{
    if (GTK_IS_CONTAINER (node)) {
        GList *children = gtk_container_get_children (GTK_CONTAINER (node));
        GomNode *child = children ? children->data : NULL;
        g_list_free (children);
        return child;
    }
    return NULL;
}

static GomNode *
widget_get_last_child (GomNode *node)
{
    if (GTK_IS_CONTAINER (node)) {
        GList *children = gtk_container_get_children (GTK_CONTAINER (node));
        GList *last = g_list_last (children);
        GomNode *child = last ? last->data : NULL;
        g_list_free (children);
        return child;
    }
    return NULL;
}

static GomNode *
widget_get_previous_sibling (GomNode *node)
{
    GomNode *sib = NULL;
    GtkWidget *parent = gtk_widget_get_parent (GTK_WIDGET (node));
    if (parent && GTK_IS_CONTAINER (parent)) {
        GList *children = gtk_container_get_children (GTK_CONTAINER (parent));
        GList *self = g_list_find (children, GTK_WIDGET (node));
        if (self) {
            sib = self && self->prev ? GOM_NODE (self->prev->data) : NULL;
        }
        g_list_free (children);
    }
    return sib;
}

static GomNode *
widget_get_next_sibling (GomNode *node)
{
    GomNode *sib = NULL;
    GtkWidget *parent = gtk_widget_get_parent (GTK_WIDGET (node));
    if (parent && GTK_IS_CONTAINER (parent)) {
        GList *children = gtk_container_get_children (GTK_CONTAINER (parent));
        GList *self = g_list_find (children, GTK_WIDGET (node));
        if (self) {
            sib = self && self->next ? GOM_NODE (self->next->data) : NULL;
        }
        g_list_free (children);
    }
    return sib;
}

static GomNamedNodeMap *
widget_get_attributes (GomNode *node)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomDocument *
widget_get_owner_document (GomNode *node)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomNode *
widget_insert_before (GomNode *node,
                      GomNode *new_child,
                      GomNode *ref_child,
                      GError  **error)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomNode *
widget_replace_child (GomNode *node,
                      GomNode *new_child,
                      GomNode *ref_child,
                      GError  **error)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomNode *
widget_remove_child (GomNode *node,
                     GomNode *old_child,
                     GError  **error)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

typedef struct {
    GomNode *node;
    GomNode *new_child;
    GError **error;
} AppendChildData;

static void
append_child_attrs_foreach (gpointer key, gpointer value, gpointer user_data)
{
    AppendChildData *data = user_data;
    GParamSpec *spec;
    GError *error = NULL;
    GValue gval = { 0 };

    spec = gtk_container_class_find_child_property (G_OBJECT_GET_CLASS (data->node), key);
    if (!spec ||
        G_TYPE_FUNDAMENTAL (G_PARAM_SPEC_VALUE_TYPE (spec)) == G_TYPE_OBJECT) {
        return;
    }
    g_print ("found child property %s.%s on %s\n",
             g_type_name (G_TYPE_FROM_INSTANCE (data->node)),
             spec->name,
             g_type_name (G_TYPE_FROM_INSTANCE (data->new_child)));
    
    if (gtk_builder_value_from_string (NULL, spec, g_value_get_string (value), &gval, &error)) {
        gtk_container_child_set_property (GTK_CONTAINER (data->node),
                                          GTK_WIDGET (data->new_child),
                                          spec->name, 
                                          &gval);
        g_value_unset (&gval);
    } else {
        g_print ("Error getting value_from_string: %s\n",
                 error->message);
        if (data->error && *data->error) {
            g_error_free (error);
        } else {
            g_propagate_error (data->error, error);
        }
    }
}

static GomNode *
widget_append_child (GomNode *node,
                     GomNode *new_child,
                     GError  **error)
{
    AppendChildData data;

    if (!GTK_IS_CONTAINER (node)) {
        g_set_error (error,
                     GOM_DOM_EXCEPTION_ERROR,
                     GOM_HIERCHY_REQUEST_ERR,
                     "Cannot append a child that is not a GtkWidget");
        return NULL;
    }

    if (!GTK_IS_WIDGET (new_child)) {
        g_set_error (error,
                     GOM_DOM_EXCEPTION_ERROR,
                     GOM_HIERCHY_REQUEST_ERR,
                     "Cannot append to a parent that is not a GtkContainer");
        return NULL;
    }

    gtk_container_add (GTK_CONTAINER (node), GTK_WIDGET (new_child));

    data.node = node;
    data.new_child = new_child;
    data.error = error;

    gom_object_attributes_foreach (G_OBJECT (new_child), append_child_attrs_foreach, &data);

    return error && *error ? NULL : new_child;
}

static gboolean
widget_has_child_nodes (GomNode *node)
{
    gboolean has_child_nodes = FALSE;

    if (GTK_IS_CONTAINER (node)) {
        GList *children = gtk_container_get_children (GTK_CONTAINER (node));
        has_child_nodes = g_list_length (children) > 0;
        g_list_free (children);
    }
    return has_child_nodes;
}

static GomNode *
widget_clone_node (GomNode *node, gboolean deep)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static void
widget_normalize (GomNode *node)
{
    GOM_NOT_IMPLEMENTED;
}

static void
widget_impl_node (gpointer g_iface, gpointer iface_data)
{
    GomNodeInterface *node = (GomNodeInterface *)g_iface;

#define IFACE(func) node->func = widget_##func

    IFACE (insert_before);
    IFACE (replace_child);
    IFACE (remove_child);
    IFACE (append_child);
    IFACE (has_child_nodes);
    IFACE (clone_node);
    IFACE (normalize);

#undef IFACE
}

/* GomElement implementation */

static char *
widget_get_attribute (GomElement *elem, const char *name)
{
    GParamSpec *spec;
    GValue gval = { 0 };
    GValue *gvalp;
    char *ret = NULL;

    spec = g_object_class_find_property (G_OBJECT_GET_CLASS (elem), name);
    if (spec) {
        g_value_init (&gval, G_TYPE_STRING);
        g_object_get_property (G_OBJECT (elem), name, &gval);
        if (G_VALUE_HOLDS (&gval, G_TYPE_STRING)) {
            ret = g_value_dup_string (&gval);
        }
        g_value_unset (&gval);
    } else {
        gvalp = gom_object_get_attribute (G_OBJECT (elem), name);
        if (gvalp && G_VALUE_HOLDS_STRING (gvalp)) {
            ret = g_value_dup_string (gvalp);
        }
        g_value_unset (gvalp);
    }

    return ret;
}

static void
widget_set_attribute (GomElement *elem, const char *name, const char *value, GError **error)
{
    GParamSpec *spec;
    GValue gval = { 0 };

    spec = g_object_class_find_property (G_OBJECT_GET_CLASS (elem), name);
    if (spec) {
        if (G_TYPE_FUNDAMENTAL (G_PARAM_SPEC_VALUE_TYPE (spec)) == G_TYPE_OBJECT) {
            g_set_error (error, GOM_DOM_EXCEPTION_ERROR,
                         GOM_INVALID_ATTRIBUTE_TYPE,
                         "Attribute %s.%s is a %s, which a string cannot be converted to",
                         g_type_name (spec->owner_type), name,
                         g_type_name (G_PARAM_SPEC_VALUE_TYPE (spec)));
        } else if (gtk_builder_value_from_string (NULL, spec, value, &gval, error)) {
            g_object_set_property (G_OBJECT (elem), name, &gval);
        }
    } else {
        g_value_init (&gval, G_TYPE_STRING);
        g_value_set_string (&gval, value);
        gom_object_set_attribute (G_OBJECT (elem), name, &gval);
    }
    g_value_unset (&gval);
}

static void
widget_remove_attribute (GomElement *elem, const char *name, GError **error)
{
    GOM_NOT_IMPLEMENTED;
}

static GomAttr *
widget_get_attribute_node (GomElement *elem, const char *name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomAttr *
widget_set_attribute_node (GomElement *elem, GomAttr *new_atr, GError **error)
{
    GOM_NOT_IMPLEMENTED;
    return new_atr;
}

static GomAttr *
widget_remove_attribute_node (GomElement *elem, GomAttr *old_attr, GError **error)
{
    GOM_NOT_IMPLEMENTED;
    return old_attr;
}

static GomNodeList *
widget_get_elements_by_tag_name (GomElement *elem, const char *name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static void
gom_widget_get_property (GObject    *object,
                         guint       property_id,
                         GValue     *value,
                         GParamSpec *pspec)
{
    switch (property_id) {
    case PROP_TAG_NAME:
    case PROP_NODE_NAME:
    case PROP_LOCAL_NAME:
        g_value_set_static_string (value, g_type_name (G_TYPE_FROM_INSTANCE (object)));
        break;
    case PROP_NODE_VALUE:
        GOM_NOT_IMPLEMENTED;
        break;
    case PROP_NODE_TYPE:
        g_value_set_uint (value, GOM_ELEMENT_NODE);
        break;
    case PROP_PARENT_NODE:
        g_value_set_object (value, gtk_widget_get_parent (GTK_WIDGET (object)));
        break;
    case PROP_CHILD_NODES:
        g_value_set_object (value, 
                            GTK_IS_CONTAINER (object)
                            ? gom_g_list_new (gtk_container_get_children (GTK_CONTAINER (object)))
                            : NULL);
        break;
    case PROP_FIRST_CHILD:
        g_value_set_object (value, widget_get_first_child (GOM_NODE (object)));
        break;
    case PROP_LAST_CHILD:
        g_value_set_object (value, widget_get_last_child (GOM_NODE (object)));
        break;
    case PROP_PREVIOUS_SIBLING:
        g_value_set_object (value, widget_get_previous_sibling (GOM_NODE (object)));
        break;
    case PROP_NEXT_SIBLING:
        g_value_set_object (value, widget_get_next_sibling (GOM_NODE (object)));
        break;
    case PROP_ATTRIBUTES:
        g_value_set_object (value, widget_get_attributes (GOM_NODE (object)));
        break;
    case PROP_OWNER_DOCUMENT:
        g_value_set_object (value, widget_get_owner_document (GOM_NODE (object)));
        break;
    case PROP_NAMESPACE_URI:
    case PROP_PREFIX:
        g_value_set_string (value, NULL);
        break;
    default:
        if (_gtk_widget_get_property) {
            _gtk_widget_get_property (object, property_id, value, pspec);
        }
        break;
    }
}

static void
gom_widget_set_property (GObject      *object,
                         guint         property_id,
                         const GValue *value,
                         GParamSpec   *pspec)
{
    switch (property_id) {
    case PROP_NODE_VALUE:
        GOM_NOT_IMPLEMENTED;
        break;
    case PROP_NODE_TYPE:
    case PROP_PARENT_NODE:
    case PROP_CHILD_NODES:
    case PROP_FIRST_CHILD:
    case PROP_LAST_CHILD:
    case PROP_PREVIOUS_SIBLING:
    case PROP_NEXT_SIBLING:
    case PROP_ATTRIBUTES:
    case PROP_OWNER_DOCUMENT:
    case PROP_TAG_NAME:
    case PROP_NAMESPACE_URI:
    case PROP_PREFIX:
    case PROP_LOCAL_NAME:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    default:
        if (_gtk_widget_set_property) {
            _gtk_widget_set_property (object, property_id, value, pspec);
        }
        break;
    }
}

/*
 Triple-clicks are very similar to double-clicks, except that
 GDK_3BUTTON_PRESS is inserted after the third click. The order of the
 events is:

 1. GDK_BUTTON_PRESS
 2. GDK_BUTTON_RELEASE
 3. GDK_BUTTON_PRESS
 4. GDK_2BUTTON_PRESS
 5. GDK_BUTTON_RELEASE
 6. GDK_BUTTON_PRESS
 7. GDK_3BUTTON_PRESS
 8. GDK_BUTTON_RELEASE

Meanwhile, for the DOM...

 The click event occurs when the pointing device button is clicked
 over an element. A click is defined as a mousedown and mouseup over
 the same screen location. The sequence of these events is:

 1. mousedown
 2. mouseup
 3. click

 If multiple clicks occur at the same screen location, the sequence
 repeats with the detail attribute incrementing with each repetition.
*/

#define INIT_MOUSE_EVENT(name, f)                                       \
    G_STMT_START {                                                      \
        evt = g_object_new (GOM_TYPE_MOUSE_EVT, NULL);                  \
        gom_mouse_event_init_mouse_event (                              \
            GOM_MOUSE_EVENT (evt),                                      \
            (name), TRUE, event->type != GDK_MOTION_NOTIFY,             \
            NULL, priv->click_state,                                    \
            event->f.x_root,                                            \
            event->f.y_root,                                            \
            event->f.x,                                                 \
            event->f.y,                                                 \
            event->f.state & GDK_CONTROL_MASK,                          \
            event->f.state & GDK_MOD1_MASK,                             \
            event->f.state & GDK_SHIFT_MASK,                            \
            event->f.state & GDK_META_MASK,                             \
            (event->f.state & (GDK_BUTTON1_MASK | GDK_BUTTON2_MASK | GDK_BUTTON3_MASK)) >> 8, \
            NULL);                                                      \
    } G_STMT_END
    
#define INIT_KEY_EVENT(name)                                            \
    G_STMT_START {                                                      \
        modifiers = gom_keyboard_evt_string_from_state (event->key.state); \
        gom_keyboard_evt_lookup_keyval (event->key.keyval, &key_identifier, &key_location); \
        evt = g_object_new (GOM_TYPE_KEYBOARD_EVT, NULL);               \
        gom_keyboard_event_init_keyboard_event (                        \
            GOM_KEYBOARD_EVENT (evt),                                   \
            (name), TRUE, TRUE,                                         \
            NULL,                                                       \
            key_identifier, key_location, modifiers);                   \
        g_free (modifiers);                                             \
    } G_STMT_END

static gboolean
gom_widget_event (GtkWidget *widget, GdkEvent *event)
{
    GomEvent *evt = NULL;
    GomWidgetPrivate *priv;
    GError *error = NULL;

    char *modifiers;
    const char *key_identifier;
    GomKeyLocationCode key_location;

    switch (event->type) {
    case GDK_MOTION_NOTIFY:        
        priv = get_priv (widget);
        priv->click_state = 0;
        INIT_MOUSE_EVENT ("mousemove", motion);
        break;
    case GDK_BUTTON_PRESS:
        priv = get_priv (widget);
        ++priv->click_state;
        INIT_MOUSE_EVENT ("mousedown", button);
        break;
    case GDK_BUTTON_RELEASE:
        priv = get_priv (widget);
        INIT_MOUSE_EVENT ("mouseup", button);
        break;
    case GDK_ENTER_NOTIFY:
        priv = get_priv (widget);
        INIT_MOUSE_EVENT ("mouseover", crossing);
        break;
    case GDK_LEAVE_NOTIFY:
        priv = get_priv (widget);
        INIT_MOUSE_EVENT ("mouseout", crossing);
        break;
    case GDK_SCROLL:
        break;
    case GDK_KEY_PRESS:
        priv = get_priv (widget);
        INIT_KEY_EVENT ("keydown");
        break;
    case GDK_KEY_RELEASE:
        priv = get_priv (widget);
        INIT_KEY_EVENT ("keyup");
        break;
    default:
        break;
    }

    if (evt) {
        gom_event_target_dispatch_event (GOM_EVENT_TARGET (widget), evt, &error);
        if (error) {
            g_print ("%s:%d:%s(): Error dispatching event: %s\n",
                     __FILE__, __LINE__, __FUNCTION__, error->message);
            g_clear_error (&error);
        }
        if (gom_event_is_default_prevented (evt)) {
            g_object_unref (evt);
            return TRUE;
        }
        g_object_unref (evt);
        if (_gtk_widget_event && _gtk_widget_event (widget, event)) {
            return TRUE;
        }
        if (event->type == GDK_BUTTON_RELEASE && priv->click_state) {
            INIT_MOUSE_EVENT ("click", button);
            gom_event_target_dispatch_event (GOM_EVENT_TARGET (widget), evt, &error);
            if (error) {
                g_print ("%s:%d:%s(): Error dispatching event: %s\n",
                         __FILE__, __LINE__, __FUNCTION__, error->message);
                g_clear_error (&error);
            }
            if (gom_event_is_default_prevented (evt)) {
                g_object_unref (evt);
                return TRUE;
            }
            g_object_unref (evt);
        }
    }

    return FALSE;
}

static void
widget_impl_element (gpointer g_iface, gpointer iface_data)
{
    GomElementInterface *elem = (GomElementInterface *)g_iface;

#define IFACE(func) elem->func = widget_##func

    IFACE (get_attribute);
    IFACE (set_attribute);
    IFACE (remove_attribute);
    IFACE (get_attribute_node);
    IFACE (set_attribute_node);
    IFACE (remove_attribute_node);
    IFACE (get_elements_by_tag_name);

#undef IFACE
}

static void
widget_add_event_listener (GomEventTarget   *target,
                           const char       *type,
                           GomEventListener *listener,
                           gboolean          use_capture)
{
    GOM_NOT_IMPLEMENTED;
}

static void
widget_remove_event_listener (GomEventTarget   *target,
                              const char       *type,
                              GomEventListener *listener,
                              gboolean          use_capture)
{
    GOM_NOT_IMPLEMENTED;
}

static gboolean
widget_dispatch_event (GomEventTarget   *target,
                       GomEvent         *evt,
                       GError          **error)
{
    char *event_name;
    g_object_get (evt, "type", &event_name, NULL);
    g_print (" &&& dispatching event: %s.%s\n", g_type_name (G_TYPE_FROM_INSTANCE (target)), event_name);
    g_free (event_name);
    return FALSE;
}

static void
widget_impl_target (gpointer g_iface, gpointer iface_data)
{
    GomEventTargetInterface *target = (GomEventTargetInterface *)g_iface;

#define IFACE(func) target->func = widget_##func

    IFACE (add_event_listener);
    IFACE (remove_event_listener);
    IFACE (dispatch_event);

#undef IFACE
}

static gpointer
gom_widget_init_once (gpointer data)
{
    GObjectClass *oclass;
    GtkWidgetClass *wclass;
    GType type = 0;

    static const GInterfaceInfo node_info = {
        widget_impl_node,
        NULL, /* interface_finalize */
        NULL  /* interface_data */
    };
    static const GInterfaceInfo element_info = {
        widget_impl_element,
        NULL, /* interface_finalize */
        NULL  /* interface_data */
    };
    static const GInterfaceInfo target_info = {
        widget_impl_target,
        NULL,
        NULL,
    };

    oclass = g_type_class_ref (GTK_TYPE_WIDGET);
    wclass = GTK_WIDGET_CLASS (oclass);

    _gtk_widget_get_property = oclass->get_property;
    oclass->get_property = gom_widget_get_property;

    _gtk_widget_set_property = oclass->set_property;
    oclass->set_property = gom_widget_set_property;

    _gtk_widget_event = wclass->event;
    wclass->event = gom_widget_event;

    g_object_class_install_property (
        oclass, PROP_NODE_NAME,
        g_param_spec_string ("node-name", NULL,
                             "The name of this node, depending on its type",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_NODE_VALUE,
        g_param_spec_string ("node-value", NULL,
                             "The value of this node, depending on its type",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_NODE_TYPE,
        g_param_spec_uint ("node-type", NULL,
                           "A code representing the type of the underlying object",
                           GOM_ELEMENT_NODE, GOM_NOTATION_NODE,
                           GOM_ELEMENT_NODE,
                           G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_PARENT_NODE,
        g_param_spec_object ("parent-node", NULL,
                             "The parent of this node.",
                             GOM_TYPE_NODE,
                             G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_CHILD_NODES,
        g_param_spec_object ("child-nodes", NULL,
                             "A NodeList that contains all children of this node.",
                             GOM_TYPE_NODE_LIST,
                             G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_FIRST_CHILD,
        g_param_spec_object ("first-child", NULL,
                             "The first child of this node.",
                             GOM_TYPE_NODE,
                             G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_LAST_CHILD,
        g_param_spec_object ("last-child", NULL,
                             "The last child of this node.",
                             GOM_TYPE_NODE,
                             G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_PREVIOUS_SIBLING,
        g_param_spec_object ("previous-sibling", NULL,
                             "The node immediately preceding this node.",
                             GOM_TYPE_NODE,
                             G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_NEXT_SIBLING,
        g_param_spec_object ("next-sibling", NULL,
                             "The node immediately following this node.",
                             GOM_TYPE_NODE,
                             G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_ATTRIBUTES,
        g_param_spec_object ("attributes", NULL,
                             "A NamedNodeMap containing the attributes of this node (if it is an Element) or null otherwise.",
                             GOM_TYPE_NAMED_NODE_MAP,
                             G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_OWNER_DOCUMENT,
        g_param_spec_object ("owner-document", NULL,
                             "The Document object associated with this node.",
                             GOM_TYPE_DOCUMENT,
                             G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_NAMESPACE_URI,
        g_param_spec_string ("namespace-uri", "namespace-u-r-i",
                             "The namespace URI of this node, or null if it is unspecified.",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_PREFIX,
        g_param_spec_string ("prefix", NULL,
                             "The namespace prefix of this node, or null if it is unspecified.",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_LOCAL_NAME,
        g_param_spec_string ("local-name", NULL,
                             "Returns the local part of the qualified name of this node.",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_TAG_NAME,
        g_param_spec_string ("tag-name", NULL,
                             "The name of the element.",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

    g_type_add_interface_static (GTK_TYPE_WIDGET, GOM_TYPE_NODE, &node_info);
    g_type_add_interface_static (GTK_TYPE_WIDGET, GOM_TYPE_ELEMENT, &element_info);
    g_type_add_interface_static (GTK_TYPE_WIDGET, GOM_TYPE_EVENT_TARGET, &target_info);

#define WIDGET(w) type ^= (w);
#include "gomwidgets.c"
#undef WIDGET

    return GSIZE_TO_POINTER (type);
}

void
gom_widget_init (void)
{
    static GOnce init_once = G_ONCE_INIT;
    g_once (&init_once, gom_widget_init_once, NULL);
}
