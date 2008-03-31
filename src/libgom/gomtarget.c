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

#include "gom/gomtarget.h"

#include "gom/dom/gomcustomevent.h"
#include "gom/dom/gomeventtarget.h"
#include "gom/dom/gomnode.h"
#include "gom/gomevt.h"

#include "gommacros.h"

#include <string.h>

enum {
    PROP_TARGET = 1
};

typedef struct {
    char             *namespace_uri;
    char             *type_name;
    GomEventListener *listener;
    gboolean          use_capture;
} ListenerEntry;

typedef struct {
    GomEventTarget *target;
    GSList         *handlers;
} GomTargetPrivate;

#define PRIV(i) G_TYPE_INSTANCE_GET_PRIVATE ((i), GOM_TYPE_TARGET, GomTargetPrivate)

static GSList *
find_entry (GSList           *li, 
            const char       *namespace_uri,
            const char       *type,
            GomEventListener *listener,
            gboolean          use_capture)
{
    ListenerEntry *entry;

    while (li) {
        entry = li->data;
        if (use_capture != entry->use_capture) {
            goto next_entry;
        }
        /* listener will be NULL if we are dispatching rather than
         * looking up a specific entry for removal */
        if (listener && listener != entry->listener) {
            goto next_entry;
        }
        /* if either are NULL, no namespace matching is performed */
        if (namespace_uri && entry->namespace_uri && strcmp (namespace_uri, entry->namespace_uri)) {
            goto next_entry;
        }
        if (strcmp (type, entry->type_name)) {
            goto next_entry;
        }
        return li;
    next_entry:
        li = li->next;
    }
    return NULL;
}

static void
entry_free (gpointer data)
{
    ListenerEntry *e = data;

    g_free (e->namespace_uri);
    g_free (e->type_name);
    g_object_unref (e->listener);
    g_free (e);
}

static void
gom_target_set_property (GObject      *object,
                         guint         property_id,
                         const GValue *value,
                         GParamSpec   *pspec)
{
    GomTargetPrivate *priv = PRIV (object);

    switch (property_id) {
    case PROP_TARGET:
        priv->target = g_value_get_object (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
gom_target_add_event_listener_ns (GomEventTarget   *event_target,
                                  const char       *namespace_uri,
                                  const char       *type,
                                  GomEventListener *listener,
                                  gboolean          use_capture,
                                  GObject          *evt_group)
{
    GomTargetPrivate *priv = PRIV (event_target);
    ListenerEntry *entry;
    GSList *li;

    li = find_entry (priv->handlers, namespace_uri, type, listener, use_capture);
    if (li) {
        return;
    }

    entry = g_new0 (ListenerEntry, 1);
    entry->namespace_uri = g_strdup (namespace_uri);
    entry->type_name     = g_strdup (type);
    entry->listener      = g_object_ref (listener);
    entry->use_capture   = use_capture;

    priv->handlers = g_slist_append (priv->handlers, entry);
}

static void
gom_target_add_event_listener (GomEventTarget   *target,
                               const char       *type,
                               GomEventListener *listener,
                               gboolean          use_capture)
{
    gom_target_add_event_listener_ns (target, NULL, type, listener, use_capture, NULL);
}

static void
gom_target_remove_event_listener_ns (GomEventTarget   *target,
                                     const char       *namespace_uri,
                                     const char       *type,
                                     GomEventListener *listener,
                                     gboolean          use_capture)
{
    GomTargetPrivate *priv = PRIV (target);
    GSList *li;

    li = find_entry (priv->handlers, namespace_uri, type, listener, use_capture);
    if (li) {
        entry_free (li->data);
        priv->handlers = g_slist_delete_link (priv->handlers, li);
    }
}

static void
gom_target_remove_event_listener (GomEventTarget   *target,
                                  const char       *type,
                                  GomEventListener *listener,
                                  gboolean          use_capture)
{
    gom_target_remove_event_listener_ns (target, NULL, type, listener, use_capture);
}

static gboolean
gom_target_has_event_listener_ns (GomEventTarget   *target,
                                  const char       *namespace_uri,
                                  const char       *type)
{
    GomTargetPrivate *priv = PRIV (target);
    return find_entry (priv->handlers, namespace_uri, type, NULL, TRUE) ||
        find_entry (priv->handlers, namespace_uri, type, NULL, FALSE);
}

static gboolean
gom_target_will_trigger_ns (GomEventTarget   *target,
                            const char       *namespace_uri,
                            const char       *type)
{
    GomTargetPrivate *priv = PRIV (target);
    GomNode *parent;
    if (gom_target_has_event_listener_ns (target, namespace_uri, type)) {
        return TRUE;
    }
    if (priv->target) {
        target = priv->target;
    }
    g_object_get (target, "parent", &parent, NULL);
    if (parent && !GOM_IS_EVENT_TARGET (parent)) {
        g_warning ("Parent is not an EventTarget.");
        return FALSE;
    }
    return parent ? gom_event_target_will_trigger_ns (GOM_EVENT_TARGET (parent), namespace_uri, type) : FALSE;
}

static void
dispatch_events (GomEventTarget *target,
                 GomEvent       *evt,
                 const char     *namespace_uri,
                 const char     *type_name,
                 GomPhaseType    phase)
{
    GomTarget *del = NULL;
    ListenerEntry *entry;
    GSList *li;
    
    if (GOM_IS_TARGET (target)) {
        del = GOM_TARGET (target);
    } else if (GOM_IS_TARGET_PROXY (target)) {
        del = gom_target_proxy_get_target (GOM_TARGET_PROXY (target));
    }
    if (!del) {
        char *name;
        g_object_get (target, "node-name", &name, NULL);
        g_warning ("%s:%d:%s(): Node %s w/o a delegate, skipping.",
                   __FILE__, __LINE__, __FUNCTION__, name);
        g_free (name);
        return;
    }
    
    li = PRIV (del)->handlers;
    while ((li = find_entry (li, namespace_uri, type_name, NULL, phase == GOM_CAPTURING_PHASE))) {
        entry = li->data;
        gom_custom_event_set_dispatch_state (GOM_CUSTOM_EVENT (evt), target, phase);
        gom_event_listener_handle_event (entry->listener, evt);
        if (gom_custom_event_is_immediate_propagation_stopped (GOM_CUSTOM_EVENT (evt))) {
            return;
        }
        li = li->next;
    }
}

static gboolean
gom_target_dispatch_event (GomEventTarget *target,
                           GomEvent       *evt,
                           GError        **error)
{
    GomTargetPrivate *priv = PRIV (target);
    char *namespace_uri, *type_name;
    gboolean bubbles;
    GList  *parents, *li;
    GomNode *node;
    
    if (!GOM_IS_EVT (evt) || !GOM_IS_CUSTOM_EVENT (evt)) {
        g_warning ("I cannot work with this Event.");
        return FALSE;
    }

    if (priv->target) {
        target = priv->target;
    }

    /* this happens to ref our target for us */
    gom_evt_set_target (GOM_EVT (evt), target);

    g_object_get (evt,
                  "namespace-u-r-i", &namespace_uri,
                  "type",            &type_name,
                  "bubbles",         &bubbles,
                  NULL);

    parents = NULL;
    for (g_object_get (target, "parent", &node, NULL); node; g_object_get (node, "parent", &node, NULL)) {
        parents = g_list_prepend (parents, g_object_ref (node));
    }
    
    /* CAPTURING_PHASE */
    for (li = parents; li; li = li->next) {
        dispatch_events (GOM_EVENT_TARGET (li->data), evt, namespace_uri, type_name, GOM_CAPTURING_PHASE);
        if (gom_custom_event_is_propagation_stopped (GOM_CUSTOM_EVENT (evt)) ||
            gom_custom_event_is_immediate_propagation_stopped (GOM_CUSTOM_EVENT (evt))) {
            goto dispatch_out;
        }
    }

    /* AT_TARGET */
    dispatch_events (target, evt, namespace_uri, type_name, GOM_AT_TARGET);
    if (gom_custom_event_is_propagation_stopped (GOM_CUSTOM_EVENT (evt)) ||
        gom_custom_event_is_immediate_propagation_stopped (GOM_CUSTOM_EVENT (evt))) {
        goto dispatch_out;
    }

    /* BUBBLING_PHASE */
    if (bubbles) {
        for (li = g_list_last (parents); li; li = li->prev) {
            dispatch_events (GOM_EVENT_TARGET (li->data), evt, namespace_uri, type_name, GOM_BUBBLING_PHASE);
            if (gom_custom_event_is_propagation_stopped (GOM_CUSTOM_EVENT (evt)) ||
                gom_custom_event_is_immediate_propagation_stopped (GOM_CUSTOM_EVENT (evt))) {
                goto dispatch_out;
            }
        }
    }

dispatch_out:
    g_free (namespace_uri);
    g_free (type_name);
    g_list_foreach (parents, (GFunc)g_object_unref, NULL);
    g_list_free (parents);

    return gom_event_is_default_prevented (evt);
}

GOM_IMPLEMENT (EVENT_TARGET, event_target, gom_target);

G_DEFINE_TYPE_WITH_CODE (GomTarget, gom_target, G_TYPE_OBJECT,
                         GOM_IMPLEMENT_INTERFACE (EVENT_TARGET, event_target, gom_target));

static void
gom_target_finalize (GObject *object)
{
    GomTargetPrivate *priv = PRIV (object);
    g_print ("%s:%d:%s (%s %p)\n",
             __FILE__, __LINE__, __FUNCTION__,
             g_type_name (G_TYPE_FROM_INSTANCE (object)), object);

    if (priv->handlers) {
        g_slist_foreach (priv->handlers, (GFunc)entry_free, NULL);
        g_slist_free (priv->handlers);
        priv->handlers = NULL;
    }

    G_OBJECT_CLASS (gom_target_parent_class)->finalize (object);
}

static void gom_target_init (GomTarget *del) { }

static void
gom_target_class_init (GomTargetClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (GomTargetPrivate));

    oclass->set_property = gom_target_set_property;
    oclass->finalize     = gom_target_finalize;

    g_object_class_install_property (
        oclass, PROP_TARGET,
        g_param_spec_object ("target", NULL,
                             "The EventTarget of which this is a delegate.",
                             GOM_TYPE_EVENT_TARGET,
                             G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));
}

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE(GomTargetProxy, gom_target_proxy, {}, GOM_TYPE_EVENT_TARGET);

GOM_STUB_FUNC (GOM_TARGET_PROXY, gom_target_proxy, get_target,
               (GomTargetProxy *gom_target_proxy), (gom_target_proxy),
               GomTarget *);
