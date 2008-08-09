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

#include "gom/gomwidget.h"

#include "gom/dom/gomdocumentevent.h"
#include "gom/dom/gomdombuiltins.h"
#include "gom/dom/gomdomexception.h"
#include "gom/dom/gomelement.h"
#include "gom/dom/gomeventtarget.h"
#include "gom/dom/gomkeyboardevent.h"
#include "gom/dom/gommouseevent.h"
#include "gom/dom/gomuievent.h"
#include "gom/gomeventtargetinternal.h"
#include "gom/gomglist.h"
#include "gom/gomjselement.h"
#include "gom/gomjsobject.h"
#include "gom/gomkeyboardevt.h"
#include "gom/gomlistenerlist.h"
#include "gom/gomnodeinternal.h"
#include "gom/gomobject.h"

#include "gommacros.h"

#include <gtk/gtk.h>

#include <strings.h>
#include <stdlib.h>
#include <limits.h>

enum {
    PROP_TAG_NAME = 1078,
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
};

static void (*_gtk_widget_set_property) (GObject        *object,
                                         guint           property_id,
                                         const GValue   *value,
                                         GParamSpec     *pspec);
static void (*_gtk_widget_get_property) (GObject        *object,
                                         guint           property_id,
                                         GValue         *value,
                                         GParamSpec     *pspec);

static void     (*_gtk_widget_constructed) (GObject *object);

GQuark gom_widget_private_quark (void);
GOM_DEFINE_QUARK (widget_private);

typedef struct {
    GomDocument     *owner_document;
    GomListenerList *listeners;
    GomNode         *next_sibling;
    GomNode         *parent_node;
    GomNode         *prev_sibling;
    char            *namespace_uri;
    char            *prefix;
    char            *tag_name;
    GdkRectangle     allocation;
    guint            click_state;
    guint            constructed    : 1;
    guint            dirty_children : 1;
} GomWidgetPrivate;

static void
free_priv (gpointer data)
{
    GomWidgetPrivate *priv = data;
    if (priv->tag_name) {
        g_print (GOM_LOC ("Freeing a <%s>'s privates\n"), priv->tag_name);
    }
    GOM_UNSET_WEAK (priv->owner_document);
    gom_listener_list_free (priv->listeners);
    g_free (priv->namespace_uri);
    g_free (priv->prefix);
    g_free (priv->tag_name);
    g_free (priv);
}

static GomWidgetPrivate *
get_priv (gpointer obj)
{
    GomWidgetPrivate *priv;
    priv = g_object_get_qdata (obj, gom_widget_private_quark ());
    if (!priv) {
        priv = g_new0 (GomWidgetPrivate, 1);
        priv->listeners = gom_listener_list_new ();
        g_object_set_qdata_full (obj, gom_widget_private_quark (), priv, free_priv);
    }
    return priv;
}
#define PRIV(i) get_priv (i)

static GomNode *
widget_insert_before (GomNode *node,
                      GomNode *new_child,
                      GomNode *ref_child,
                      GError  **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomNode *
widget_replace_child (GomNode *node,
                      GomNode *new_child,
                      GomNode *ref_child,
                      GError  **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomNode *
widget_remove_child (GomNode *node,
                     GomNode *old_child,
                     GError  **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
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
#if 0
    g_print ("found child property %s.%s on %s\n",
             g_type_name (G_TYPE_FROM_INSTANCE (data->node)),
             spec->name,
             g_type_name (G_TYPE_FROM_INSTANCE (data->new_child)));
#endif
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
                     "Cannot append to a parent that is not a GtkContainer");
        return new_child;
    }
    if (!GTK_IS_WIDGET (new_child)) {
        if (!GOM_IS_TEXT (new_child)) {
            g_set_error (error,
                         GOM_DOM_EXCEPTION_ERROR,
                         GOM_HIERCHY_REQUEST_ERR,
                         "Cannot append a child that is not a GtkWidget");
        }
        return new_child;
    }

    gtk_container_add (GTK_CONTAINER (node), GTK_WIDGET (new_child));

    data.node = node;
    data.new_child = new_child;
    data.error = error;

    gom_object_attributes_foreach (G_OBJECT (new_child), append_child_attrs_foreach, &data);

    if (GOM_IS_NODE_INTERNAL (new_child)) {
        gom_node_internal_set_parent (GOM_NODE_INTERNAL (new_child), node);
    }

    /* gtk_container_add() doesn't ref, but append_child() should, so
     * ref our child */
    return g_object_ref (new_child);
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

static gboolean
widget_is_supported (GomNode    *node,
                     const char *feature,
                     const char *version)
{
    GOM_NOT_IMPLEMENTED;
    return FALSE;
}

static gboolean
widget_has_attributes (GomNode *node)
{
    return TRUE;
}

static char *
widget_get_attribute_ns (GomElement *elem, const char *namespace_uri, const char *name)
{
    GParamSpec *spec;
    GValue gval = { 0 };
    const GValue *gvalp;
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
        if (gvalp) {
            if (G_VALUE_HOLDS_STRING (gvalp)) {
                ret = g_value_dup_string (gvalp);
            }
        }
    }

    return ret;
}

static char *
widget_get_attribute (GomElement *elem, const char *name)
{
    return widget_get_attribute_ns (elem, NULL, name);
}

static void
widget_set_attribute_ns (GomElement *elem,
                         const char *namespace_uri,
                         const char *qualified_name,
                         const char *value,
                         GError **error)
{
    GParamSpec *spec;
    GValue gval = { 0 };

    spec = g_object_class_find_property (G_OBJECT_GET_CLASS (elem), qualified_name);
    if (spec) {
        if (G_TYPE_FUNDAMENTAL (G_PARAM_SPEC_VALUE_TYPE (spec)) == G_TYPE_OBJECT) {
            g_set_error (error, GOM_DOM_EXCEPTION_ERROR,
                         GOM_INVALID_ATTRIBUTE_TYPE_ERR,
                         "Attribute %s.%s is a %s, which a string cannot be converted to",
                         g_type_name (spec->owner_type), qualified_name,
                         g_type_name (G_PARAM_SPEC_VALUE_TYPE (spec)));
        } else if (gtk_builder_value_from_string (NULL, spec, value, &gval, error)) {
            g_object_set_property (G_OBJECT (elem), qualified_name, &gval);
        }
    } else {
        g_value_init (&gval, G_TYPE_STRING);
        g_value_set_string (&gval, value);
        gom_object_set_attribute (G_OBJECT (elem), qualified_name, &gval);
    }
    g_value_unset (&gval);
}

static void
widget_set_attribute (GomElement *elem, const char *name, const char *value, GError **error)
{
    widget_set_attribute_ns (elem, NULL, name, value, error);
}

static void
widget_remove_attribute_ns (GomElement *elem,
                                const char *namespace_uri,
                                const char *local_name,
                                GError    **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
}

static void
widget_remove_attribute (GomElement *elem, const char *name, GError **error)
{
    widget_remove_attribute_ns (elem, NULL, name, error);
}

static GomAttr *
widget_get_attribute_node_ns (GomElement *elem,
                              const char *namespace_uri,
                              const char *local_name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomAttr *
widget_get_attribute_node (GomElement *elem, const char *name)
{
    return widget_get_attribute_node_ns (elem, NULL, name);
}

static GomAttr *
widget_set_attribute_node_ns (GomElement *elem, GomAttr *new_atr, GError **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return new_atr;
}

static GomAttr *
widget_set_attribute_node (GomElement *elem, GomAttr *new_atr, GError **error)
{
    return widget_set_attribute_node_ns (elem, new_atr, error);
}

static GomAttr *
widget_remove_attribute_node (GomElement *elem, GomAttr *old_attr, GError **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return old_attr;
}

static GomNodeList *
widget_get_elements_by_tag_name_ns (GomElement *elem, const char *namespace_uri, const char *name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomNodeList *
widget_get_elements_by_tag_name (GomElement *elem, const char *name)
{
    return widget_get_elements_by_tag_name_ns (elem, NULL, name);
}

static gboolean
widget_has_attribute_ns (GomElement *elem, const char *namespace_uri, const char *local_name)
{
    GOM_NOT_IMPLEMENTED;
    return FALSE;
}    

static gboolean
widget_has_attribute (GomElement *elem, const char *name)
{
    return widget_has_attribute_ns (elem, NULL, name);
}

static void
gom_widget_get_property (GObject    *object,
                         guint       property_id,
                         GValue     *value,
                         GParamSpec *pspec)
{
    GomWidgetPrivate *priv = PRIV (object);
    GList *children = NULL;
    GList *li = NULL;

    switch (property_id) {
    case PROP_TAG_NAME:
        g_value_set_string (value, priv->tag_name);
        break;
    case PROP_NODE_VALUE:
        g_value_set_string (value, NULL);
        break;
    case PROP_NODE_TYPE:
        g_value_set_enum (value, GOM_ELEMENT_NODE);
        break;
    case PROP_OWNER_DOCUMENT:
        g_value_set_object (value, priv->owner_document);
        break;
    case PROP_NAMESPACE_URI:
        g_value_set_string (value, priv->namespace_uri);
        break;
    case PROP_PREFIX:
        g_value_set_string (value, priv->prefix);
        break;
    case PROP_LOCAL_NAME:
        g_value_set_static_string (value, g_type_name (G_TYPE_FROM_INSTANCE (object)));
        break;
    case PROP_PARENT_NODE:
        g_value_set_object (value, priv->parent_node);
        break;
    case PROP_CHILD_NODES: {
	GomNodeList *nodes;
	GList *li;
	li = GTK_IS_CONTAINER (object)
	    ? gtk_container_get_children (GTK_CONTAINER (object))
	    : NULL;
	nodes = g_object_new (GOM_TYPE_G_LIST, "nodes", li, NULL);
	g_value_set_object (value, nodes);
	g_object_unref (nodes);
	g_list_free (li);
	break;
    }
    case PROP_FIRST_CHILD:
        if (GTK_IS_CONTAINER (object)) {
            children = gtk_container_get_children (GTK_CONTAINER (object));
        }
        g_value_set_object (value, children ? children->data : NULL);
        g_list_free (children);
        break;
    case PROP_LAST_CHILD:
        if (GTK_IS_CONTAINER (object)) {
            children = gtk_container_get_children (GTK_CONTAINER (object));
            li = g_list_last (children);
        }
        g_value_set_object (value, li ? li->data : NULL);
        g_list_free (children);
        break;
    case PROP_PREVIOUS_SIBLING:
        if (GOM_IS_NODE_INTERNAL (priv->parent_node)) {
            gom_node_internal_sibling_requested (GOM_NODE_INTERNAL (priv->parent_node), GOM_NODE (object));
        }
        g_value_set_object (value, priv->prev_sibling);
        break;
    case PROP_NEXT_SIBLING:
        if (GOM_IS_NODE_INTERNAL (priv->parent_node)) {
            gom_node_internal_sibling_requested (GOM_NODE_INTERNAL (priv->parent_node), GOM_NODE (object));
        }
        g_value_set_object (value, priv->next_sibling);
        break;
    case PROP_ATTRIBUTES:
        g_value_set_object (value, NULL);
        break;
    default:
        if (_gtk_widget_get_property) {
            _gtk_widget_get_property (object, property_id, value, pspec);
        }
        break;
    }
}

static void
update_tag_name (GObject *object)
{
    GomWidgetPrivate *priv = PRIV (object);
    if (priv->namespace_uri) {
        if (priv->prefix && !strcmp (priv->prefix, "xml") &&
            strcmp (priv->namespace_uri, "http://www.w3.org/XML/1998/namespace")) {
            g_warning (GOM_LOC ("invalid prefix '%s' for namespace '%s' on %p"),
                       priv->prefix, priv->namespace_uri, object);
            g_free (priv->prefix);
            priv->prefix = NULL;
            return;
        }
    } else if (priv->prefix) {
        g_warning (GOM_LOC ("prefix set without namespace on %p"), object);
        return;
    } else {
        return;
    }
    g_free (priv->tag_name);
    priv->tag_name = priv->prefix 
        ? g_strconcat (priv->prefix, ":", g_type_name (G_TYPE_FROM_INSTANCE (object)), NULL)
        : g_strdup (g_type_name (G_TYPE_FROM_INSTANCE (object)));
}

static void
gom_widget_set_property (GObject      *object,
                         guint         property_id,
                         const GValue *value,
                         GParamSpec   *pspec)
{
    GomWidgetPrivate *priv = PRIV (object);

    switch (property_id) {
    case PROP_OWNER_DOCUMENT:
        GOM_SET_WEAK (priv->owner_document, g_value_get_object (value));
        break;
    case PROP_NAMESPACE_URI:
        priv->namespace_uri = g_value_dup_string (value);
        break;
    case PROP_PREFIX:
        g_free (priv->prefix);
        priv->prefix = g_value_dup_string (value);
        if (priv->constructed) {
            update_tag_name (object);
        }
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
        if (GOM_IS_DOCUMENT_EVENT (priv->owner_document)) {             \
            evt = gom_document_event_create_event (                     \
                GOM_DOCUMENT_EVENT (priv->owner_document),              \
                "MouseEvent", &error);                                  \
        }                                                               \
        if (GOM_IS_MOUSE_EVENT (evt)) {                                 \
            gom_mouse_event_init_mouse_event (                          \
                GOM_MOUSE_EVENT (evt),                                  \
                (name), TRUE, event->type != GDK_MOTION_NOTIFY,         \
                NULL, priv->click_state,                                \
                event->f.x_root,                                        \
                event->f.y_root,                                        \
                event->f.x,                                             \
                event->f.y,                                             \
                event->f.state & GDK_CONTROL_MASK,                      \
                event->f.state & GDK_MOD1_MASK,                         \
                event->f.state & GDK_SHIFT_MASK,                        \
                event->f.state & GDK_META_MASK,                         \
                (event->f.state & (GDK_BUTTON1_MASK | GDK_BUTTON2_MASK | GDK_BUTTON3_MASK)) >> 8, \
                NULL);                                                  \
        }                                                               \
    } G_STMT_END
    
#define INIT_KEY_EVENT(name)                                            \
    G_STMT_START {                                                      \
        if (GOM_IS_DOCUMENT_EVENT (priv->owner_document)) {             \
            evt = gom_document_event_create_event (                     \
                GOM_DOCUMENT_EVENT (priv->owner_document),              \
                "KeyboardEvent", &error);                               \
        }                                                               \
        if (GOM_IS_KEYBOARD_EVENT (evt)) {                              \
            modifiers = gom_keyboard_evt_string_from_state (event->key.state); \
            gom_keyboard_evt_keyval_to_key_identifier (event->key.keyval, &key_identifier, &key_location); \
            gom_keyboard_event_init_keyboard_event (                    \
                GOM_KEYBOARD_EVENT (evt),                               \
                (name), TRUE, TRUE,                                     \
                NULL,                                                   \
                key_identifier, key_location, modifiers);               \
            g_free (modifiers);                                         \
        }                                                               \
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

    static GdkEventType  last_type  = 0;
    static GdkEvent     *last_event = NULL;

    if (event == last_event && event->type == last_type) {
        return FALSE;
    }

    switch (event->type) {
    case GDK_MOTION_NOTIFY:        
        priv = PRIV (widget);
        priv->click_state = 0;
        INIT_MOUSE_EVENT (GOM_MOUSEMOVE, motion);
        break;
    case GDK_BUTTON_PRESS:
        priv = PRIV (widget);
        ++priv->click_state;
        INIT_MOUSE_EVENT (GOM_MOUSEDOWN, button);
        break;
    case GDK_BUTTON_RELEASE:
        priv = PRIV (widget);
        INIT_MOUSE_EVENT (GOM_MOUSEUP, button);
        break;
    case GDK_ENTER_NOTIFY:
        priv = PRIV (widget);
        INIT_MOUSE_EVENT (GOM_MOUSEOVER, crossing);
        break;
    case GDK_LEAVE_NOTIFY:
        priv = PRIV (widget);
        INIT_MOUSE_EVENT (GOM_MOUSEOUT, crossing);
        break;
    case GDK_KEY_PRESS:
        if (GTK_IS_WINDOW (widget) && GTK_WINDOW (widget)->focus_widget) {
            widget = GTK_WINDOW (widget)->focus_widget;
        }
        priv = PRIV (widget);
        INIT_KEY_EVENT (GOM_KEYDOWN);
        break;
    case GDK_KEY_RELEASE:
        if (GTK_IS_WINDOW (widget) && GTK_WINDOW (widget)->focus_widget) {
            widget = GTK_WINDOW (widget)->focus_widget;
        }
        priv = PRIV (widget);
        INIT_KEY_EVENT (GOM_KEYUP);
        break;
    default:
        break;
    }

    if (!evt) {
        return FALSE;
    }

    g_assert (GOM_IS_EVENT (evt));

    last_type  = event->type;
    last_event = event;

    g_object_ref (widget);
    gom_listener_list_dispatch_event (GOM_EVENT_TARGET (widget), evt, &error);
    if (error) {
        g_print (GOM_LOC ("Error dispatching event: %s\n"),
                  error->message);
        g_clear_error (&error);
    }
    if (gom_event_is_default_prevented (evt)) {
        g_object_unref (evt);
        g_object_unref (widget);
        return TRUE;
    }
    g_object_unref (evt);
    if (event->type == GDK_BUTTON_RELEASE && priv->click_state) {
        evt = NULL;
        INIT_MOUSE_EVENT (GOM_CLICK, button);
        if (!evt) {
            g_object_unref (widget);
            return FALSE;
        }
        gom_listener_list_dispatch_event (GOM_EVENT_TARGET (widget), evt, &error);
        if (error) {
            g_print (GOM_LOC ("Error dispatching event: %s\n"),
                      error->message);
            g_clear_error (&error);
        }
        if (gom_event_is_default_prevented (evt)) {
            g_object_unref (evt);
            g_object_unref (widget);
            return TRUE;
        }
        g_object_unref (evt);
    }
    g_object_unref (widget);
    return FALSE;
}

static void
gom_widget_event_after (GtkWidget *widget, GdkEvent *event)
{
    GomEvent *evt = NULL;
    GomWidgetPrivate *priv;
    GError *error = NULL;

    static GdkEventType  last_type  = 0;
    static GdkEvent     *last_event = NULL;

    if (event == last_event && event->type == last_type) {
        return;
    }

    switch (event->type) {
    case GDK_FOCUS_CHANGE:
        priv = PRIV (widget);
        if (GOM_IS_DOCUMENT_EVENT (priv->owner_document)) {
            evt = gom_document_event_create_event (GOM_DOCUMENT_EVENT (priv->owner_document),
                                                   "UIEvent", &error);
        }
        if (GOM_IS_UI_EVENT (evt)) {
            gom_ui_event_init_ui_event_ns (GOM_UI_EVENT (evt),
                                           GOM_EVENTS_NAMESPACE_URI,
                                           event->focus_change.in ? GOM_DOM_FOCUS_IN : GOM_DOM_FOCUS_OUT,
                                           TRUE, FALSE,
                                           NULL, 0);
        }
        break;
    default:
        break;
    }

    if (!evt) {
        return;
    }

    g_assert (GOM_IS_EVENT (evt));

    last_type  = event->type;
    last_event = event;

    gom_listener_list_dispatch_event (GOM_EVENT_TARGET (widget), evt, &error);
    if (error) {
        g_print (GOM_LOC ("Error dispatching event: %s\n"),
                  error->message);
        g_clear_error (&error);
    }
    g_object_unref (evt);
}

static void
widget_add_event_listener_ns (GomEventTarget   *event_target,
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
widget_add_event_listener (GomEventTarget   *target,
                           const char       *type,
                           GomEventListener *listener,
                           gboolean          use_capture)
{
    gom_listener_list_add_event_listener (
        PRIV (target)->listeners, NULL,
        type, listener, use_capture, NULL);
}

static void
widget_remove_event_listener_ns (GomEventTarget   *target,
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
widget_remove_event_listener (GomEventTarget   *target,
                              const char       *type,
                              GomEventListener *listener,
                              gboolean          use_capture)
{
    gom_listener_list_remove_event_listener (
        PRIV (target)->listeners, NULL,
        type, listener, use_capture);
}

static gboolean
widget_has_event_listener_ns (GomEventTarget   *target,
                              const char       *namespace_uri,
                              const char       *type)
{
    return gom_listener_list_has_event_listener (PRIV (target)->listeners, namespace_uri, type);
}

static gboolean
widget_will_trigger_ns (GomEventTarget   *target,
                        const char       *namespace_uri,
                        const char       *type)
{
    return gom_listener_list_will_trigger (PRIV (target)->listeners, target, namespace_uri, type);
}

static gboolean
widget_dispatch_event (GomEventTarget *target,
                       GomEvent       *evt,
                       GError        **error)
{
    GdkEvent event = { 0 };
    gboolean ctrlKey, shiftKey, altKey, metaKey;
    GdkKeymapKey *keys;
    gint n_keys;
    GomKeyLocationCode location;
    char *identifier;
    long timestamp;
    char *type;

    g_object_get (evt, "type", &type, NULL);

    if (GOM_IS_KEYBOARD_EVENT (evt)) {
        if (!strcmp (type, GOM_KEYDOWN)) {
            event.any.type = GDK_KEY_PRESS;
        } else if (!strcmp (type, GOM_KEYUP)) {
            event.any.type = GDK_KEY_RELEASE;
        } else {
            goto dispatch;
        }
        g_object_get (evt,
                      "key-identifier", &identifier,
                      "key-location",   &location,
                      "ctrl-key",       &ctrlKey,
                      "shift-key",      &shiftKey,
                      "alt-key",        &altKey,
                      "meta-key",       &metaKey,
                      "time-stamp",     &timestamp,
                      NULL);
        if (!gom_keyboard_evt_key_identifier_to_keyval (identifier, location,
                                                        &event.key.keyval)) {
            g_free (identifier);
            goto dispatch;
        }
        if (!gdk_keymap_get_entries_for_keyval (NULL, event.key.keyval, &keys, &n_keys)) {
            g_free (identifier);
            goto dispatch;
        }
        g_free (identifier);

        event.key.hardware_keycode = keys[0].keycode;
        event.key.group = keys[0].group;
        g_free (keys);

        event.key.window = GTK_WIDGET (target)->window;
        event.key.send_event = FALSE;
        event.key.time = timestamp;
        if (ctrlKey) {
            event.key.state |= GDK_CONTROL_MASK;
        }
        if (shiftKey) {
            event.key.state |= GDK_SHIFT_MASK;
        }
        if (altKey) {
            event.key.state |= GDK_MOD1_MASK;
        }
        if (metaKey) {
            event.key.state |= GDK_META_MASK;
        }
    } else if (GOM_IS_MOUSE_EVENT (evt)) {

    } else if (GOM_IS_UI_EVENT (evt)) {

    } else {

    }

    if (event.any.type) {
        g_free (type);
        gtk_main_do_event (&event);
        return TRUE;
    }
dispatch:
    g_free (type);
    return gom_listener_list_dispatch_event (target, evt, error);
}

static void
widget_set_parent (GomNodeInternal *child, GomNode *parent)
{
    GomWidgetPrivate *priv = PRIV (child);
#if 0
    char *child_name, *parent_name = NULL;
    g_object_get (child, "node-name", &child_name, NULL);
    if (parent) {
        g_object_get (parent, "node-name", &parent_name, NULL);
    }
    g_print (GOM_LOC ("<%s %p> <%s %p>\n"),
             child_name, child,
             parent_name ? parent_name : "(no parent)", parent);
    g_free (child_name);
    g_free (parent_name);
#endif
    priv->next_sibling = NULL;
    priv->prev_sibling = NULL;
    priv->parent_node  = parent;
}

static void
widget_set_next_sibling (GomNodeInternal *child, GomNode *sibling)
{
#if 0
    char *child_name, *sibling_name = NULL;
    g_object_get (child, "node-name", &child_name, NULL);
    if (sibling) {
        g_object_get (sibling, "node-name", &sibling_name, NULL);
    }
    g_print (GOM_LOC ("<%s %p> <%s %p>\n"),
             child_name, child,
             sibling_name ? sibling_name : "(no sibling)", sibling);
    g_free (child_name);
    g_free (sibling_name);
#endif
    PRIV (child)->next_sibling = sibling;
}

static void
widget_set_prev_sibling (GomNodeInternal *child, GomNode *sibling)
{
#if 0
    char *child_name, *sibling_name = NULL;
    g_object_get (child, "node-name", &child_name, NULL);
    if (sibling) {
        g_object_get (sibling, "node-name", &sibling_name, NULL);
    }
    g_print (GOM_LOC ("<%s %p> <%s %p>\n"),
             child_name, child,
             sibling_name ? sibling_name : "(no sibling)", sibling);
    g_free (child_name);
    g_free (sibling_name);
#endif
    PRIV (child)->prev_sibling = sibling;
}

static void
widget_sibling_requested (GomNodeInternal *node, GomNode *child)
{
    GomWidgetPrivate *priv = PRIV (node);
    GList *children;
    GList *li;
    g_assert (GTK_IS_CONTAINER (node));
    if (!priv->dirty_children) {
        return;
    }
    children = gtk_container_get_children (GTK_CONTAINER (node));
    for (li = children; li; li = li->next) {
        if (GOM_IS_NODE_INTERNAL (li->data)) {
            gom_node_internal_set_prev_sibling (li->data, li->prev ? li->prev->data : NULL);
            gom_node_internal_set_next_sibling (li->data, li->next ? li->next->data : NULL);
        }
    }
    g_list_free (children);
    priv->dirty_children = 0;
}

static void
widget_dispatch_listeners (GomEventTargetInternal *current_target,
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

static GomEventTarget *
widget_get_parent_target (GomEventTargetInternal *target)
{
    GomWidgetPrivate *priv = PRIV (target);

    return priv->parent_node ? GOM_EVENT_TARGET (priv->parent_node) : NULL;
}

static void
widget_dirty_children (GtkContainer *container, GtkWidget *widget, gpointer data)
{
    PRIV (container)->dirty_children = 1;
}


static void
widget_activate (GtkWidget *w, gpointer data)
{
    GomWidgetPrivate *priv = PRIV (w);
    GomEvent *event;
    GError   *error = NULL;
    if (!GOM_IS_DOCUMENT_EVENT (priv->owner_document)) {
        return;
    }
    event = gom_document_event_create_event (GOM_DOCUMENT_EVENT (priv->owner_document),
                                             "UIEvent", &error);
    if (!event) {
        g_printerr (GOM_LOC ("could not create activate events: %s\n"),
                     error->message);
        g_clear_error (&error);
        return;
    }
    if (!GOM_IS_UI_EVENT (event)) {
        g_printerr (GOM_LOC ("event is a %s, but not a UIEvent\n"),
                    g_type_name (G_TYPE_FROM_INSTANCE (event)));
        g_object_unref (event);
        return;
    }
    gom_ui_event_init_ui_event_ns (GOM_UI_EVENT (event),
                                   GOM_EVENTS_NAMESPACE_URI,
                                   GOM_DOM_ACTIVATE,
                                   TRUE, TRUE, NULL, 0);
    gom_listener_list_dispatch_event (GOM_EVENT_TARGET (w), event, &error);
    g_object_unref (event);
    if (error) {
        g_printerr (GOM_LOC ("Error dispatching event: %s\n"),
                     error->message);
        g_clear_error (&error);
    }
}

static void
widget_size_allocate (GtkWidget *widget, GdkRectangle *rect, gpointer data)
{
    GomWidgetPrivate *priv = PRIV (widget);
    GomEvent *event;
    GError *error = NULL;

    if (rect->width  == priv->allocation.width &&
        rect->height == priv->allocation.height) {
        return;
    }
    if (!GOM_IS_DOCUMENT_EVENT (priv->owner_document)) {
        return;
    }
    priv->allocation = *rect;
    event = gom_document_event_create_event (GOM_DOCUMENT_EVENT (priv->owner_document),
                                             "UIEvent", &error);
    if (!event) {
        g_printerr (GOM_LOC ("could not create activate events: %s\n"),
                     error->message);
        g_clear_error (&error);
        return;
    }
    if (!GOM_IS_UI_EVENT (event)) {
        g_printerr (GOM_LOC ("event is a %s, but not a UIEvent\n"),
                    
                    g_type_name (G_TYPE_FROM_INSTANCE (event)));
        g_object_unref (event);
        return;
    }
    gom_ui_event_init_ui_event_ns (GOM_UI_EVENT (event),
                                   GOM_EVENTS_NAMESPACE_URI,
                                   GOM_RESIZE,
                                   TRUE, FALSE, NULL, 0);
    gom_listener_list_dispatch_event (GOM_EVENT_TARGET (widget), event, &error);
    g_object_unref (event);
    if (error) {
        g_printerr (GOM_LOC ("Error dispatching event: %s\n"),
                     error->message);
        g_clear_error (&error);
    }
}

static void
widget_scroll (GtkWidget *widget, GtkAdjustment *adjust)
{
    GomWidgetPrivate *priv;
    GomEvent *event;
    GError *error = NULL;
    GtkWidget *child;

    if (GTK_IS_BIN (widget)) {
        child = gtk_bin_get_child (GTK_BIN (widget));
    } else {
        child = widget;
    }
    if (!child) {
        g_print (GOM_LOC ("returning\n"));
        return;
    }

    priv = PRIV (child);
    if (!GOM_IS_DOCUMENT_EVENT (priv->owner_document)) {
        g_print (GOM_LOC ("returning\n"));
        return;
    }
    event = gom_document_event_create_event (GOM_DOCUMENT_EVENT (priv->owner_document),
                                             "UIEvent", &error);
    if (!event) {
        g_printerr (GOM_LOC ("could not create activate events: %s\n"),
                     error->message);
        g_clear_error (&error);
        return;
    }
    if (!GOM_IS_UI_EVENT (event)) {
        g_printerr (GOM_LOC ("event is a %s, but not a UIEvent\n"),
                    g_type_name (G_TYPE_FROM_INSTANCE (event)));
        g_object_unref (event);
        return;
    }
    gom_ui_event_init_ui_event_ns (GOM_UI_EVENT (event),
                                   GOM_EVENTS_NAMESPACE_URI,
                                   GOM_SCROLL,
                                   TRUE, FALSE, NULL, 0);
    gom_listener_list_dispatch_event (GOM_EVENT_TARGET (child), event, &error);
    g_object_unref (event);
    if (error) {
        g_printerr (GOM_LOC ("Error dispatching event: %s\n"),
                     error->message);
        g_clear_error (&error);
    }
}

static void
widget_scroll_adjustment_notify (GObject *obj, GParamSpec *arg1, gpointer data)
{
    GtkAdjustment *adjust = NULL;
    g_object_get (obj, arg1->name, &adjust, NULL);
    if (adjust) {
        g_signal_connect_swapped (adjust, "value-changed", G_CALLBACK (widget_scroll), obj);
    }
}

GOM_IMPLEMENT (NODE,                  node,                  widget);
GOM_IMPLEMENT (ELEMENT,               element,               widget);
GOM_IMPLEMENT (EVENT_TARGET,          event_target,          widget);
GOM_IMPLEMENT (NODE_INTERNAL,         node_internal,         widget);
GOM_IMPLEMENT (EVENT_TARGET_INTERNAL, event_target_internal, widget);

static void
gom_widget_constructed (GObject *object)
{
    GomWidgetPrivate *priv = PRIV (object);

    if (_gtk_widget_constructed) {
        _gtk_widget_constructed (object);
    }

#if 0
    if (!priv->owner_document) {
        g_warning (GOM_LOC ("%s %p: No document set"),
                   g_type_name (G_TYPE_FROM_INSTANCE (object)),
                   object);
    }
#endif

    update_tag_name (object);

    gtk_widget_add_events (GTK_WIDGET (object), 
                           GDK_BUTTON_PRESS_MASK  | GDK_BUTTON_RELEASE_MASK |
                           GDK_ENTER_NOTIFY_MASK  | GDK_LEAVE_NOTIFY_MASK   |
                           GDK_KEY_PRESS_MASK     | GDK_KEY_RELEASE_MASK    |
                           GDK_FOCUS_CHANGE_MASK  | GDK_POINTER_MOTION_HINT_MASK);

    if (GTK_IS_CONTAINER (object)) {
        g_signal_connect (object, "add",    G_CALLBACK (widget_dirty_children), NULL);
        g_signal_connect (object, "remove", G_CALLBACK (widget_dirty_children), NULL);
    }
    if (GTK_IS_BUTTON (object)) {
        g_signal_connect (object, "clicked", G_CALLBACK (widget_activate), NULL);
    }
    if (GTK_IS_ENTRY (object)) {
        g_signal_connect (object, "activate", G_CALLBACK (widget_activate), NULL);
    }
    if (GTK_IS_WINDOW (object)) {
        g_signal_connect (object, "size-allocate", G_CALLBACK (widget_size_allocate), NULL);
    }
    if (g_object_class_find_property (G_OBJECT_GET_CLASS (object), "hadjustment") &&
        g_object_class_find_property (G_OBJECT_GET_CLASS (object), "vadjustment")) {
        g_signal_connect (object, "notify::hadjustment", G_CALLBACK (widget_scroll_adjustment_notify), NULL);
        g_signal_connect (object, "notify::vadjustment", G_CALLBACK (widget_scroll_adjustment_notify), NULL);
    }
    g_signal_connect (object, "event",       G_CALLBACK (gom_widget_event),       NULL);
    g_signal_connect (object, "event-after", G_CALLBACK (gom_widget_event_after), NULL);

    priv->constructed = 1;
}

static gpointer
gom_widget_init_once (gpointer data)
{
    GObjectClass      *oclass;
    GType type = 0;

    static const GInterfaceInfo node_info = {
        widget_implement_node,
        NULL, /* interface_finalize */
        NULL  /* interface_data */
    };
    static const GInterfaceInfo element_info = {
        widget_implement_element,
        NULL, /* interface_finalize */
        NULL  /* interface_data */
    };
    static const GInterfaceInfo target_info = {
        widget_implement_event_target,
        NULL,
        NULL,
    };
    static const GInterfaceInfo node_internal_info = {
        widget_implement_node_internal,
        NULL,
        NULL
    };
    static const GInterfaceInfo target_internal_info = {
        widget_implement_event_target_internal,
        NULL,
        NULL
    };

    oclass = g_type_class_ref (GTK_TYPE_WIDGET);

    _gtk_widget_get_property = oclass->get_property;
    oclass->get_property = gom_widget_get_property;

    _gtk_widget_set_property = oclass->set_property;
    oclass->set_property = gom_widget_set_property;

    _gtk_widget_constructed = oclass->constructed;
    oclass->constructed = gom_widget_constructed;

    g_object_class_install_property (
        oclass, PROP_TAG_NAME,
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
        g_param_spec_enum ("node-type", NULL,
                           "A code representing the type of the underlying object",
                           GOM_TYPE_NODE_TYPE, GOM_ELEMENT_NODE,
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
                             G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_NAMESPACE_URI,
        g_param_spec_string ("namespace-u-r-i", "namespace-uri",
                             "The namespace URI of this node, or null if it is unspecified.",
                             NULL,
                             G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_PREFIX,
        g_param_spec_string ("prefix", NULL,
                             "The namespace prefix of this node, or null if it is unspecified.",
                             NULL,
                             G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_LOCAL_NAME,
        g_param_spec_string ("local-name", NULL,
                             "Returns the local part of the qualified name of this node.",
                             NULL,
                             G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_TAG_NAME,
        g_param_spec_string ("tag-name", NULL,
                             "The name of the element.",
                             NULL,
                             G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_type_add_interface_static (GTK_TYPE_WIDGET, GOM_TYPE_EVENT_TARGET,          &target_info);
    g_type_add_interface_static (GTK_TYPE_WIDGET, GOM_TYPE_EVENT_TARGET_INTERNAL, &target_internal_info);
    g_type_add_interface_static (GTK_TYPE_WIDGET, GOM_TYPE_NODE,                  &node_info);
    g_type_add_interface_static (GTK_TYPE_WIDGET, GOM_TYPE_ELEMENT,               &element_info);
    g_type_add_interface_static (GTK_TYPE_WIDGET, GOM_TYPE_NODE_INTERNAL,         &node_internal_info);

#define WIDGET(w) type ^= w;
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
