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

#include "gom/gomlistenerlist.h"

#include "gom/dom/gomcustomevent.h"
#include "gom/dom/gomnode.h"
#include "gom/gomevt.h"
#include "gom/gomnodeinternal.h"

#include "gommacros.h"

#include <string.h>

typedef struct {
    char             *namespace_uri;
    char             *type_name;
    GomEventListener *listener;
    gboolean          use_capture;
} ListenerEntry;

struct _GomListenerList {
    GSList *listeners;
};

static GSList *
find_entry (GSList           *li, 
            const char       *namespace_uri,
            const char       *type,
            GomEventListener *listener,
            gboolean          use_capture)
{
    ListenerEntry *entry;

    for ( ; li; li = li->next) {
        entry = li->data;
        if (use_capture != entry->use_capture) {
            continue;
        }
        /* listener will be NULL if we are dispatching rather than
         * looking up a specific entry for removal */
        if (listener && listener != entry->listener) {
            continue;
        }
        /* if either are NULL, no namespace matching is performed */
        if (namespace_uri && entry->namespace_uri && strcmp (namespace_uri, entry->namespace_uri)) {
            continue;
        }
        if (strcmp (type, entry->type_name)) {
            continue;
        }
        break;
    }

    return li;
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

void
gom_listener_list_add_event_listener (GomListenerList  *list,
                                      const char       *namespace_uri,
                                      const char       *type,
                                      GomEventListener *listener,
                                      gboolean          use_capture,
                                      GObject          *evt_group)
{
    ListenerEntry *entry;
    GSList *li;

    li = find_entry (list->listeners, namespace_uri, type, listener, use_capture);
    if (li) {
        return;
    }

    entry = g_new0 (ListenerEntry, 1);
    entry->namespace_uri = g_strdup (namespace_uri);
    entry->type_name     = g_strdup (type);
    entry->listener      = g_object_ref (listener);
    entry->use_capture   = use_capture;

    list->listeners = g_slist_append (list->listeners, entry);
}

void
gom_listener_list_remove_event_listener (GomListenerList  *list,
                                         const char       *namespace_uri,
                                         const char       *type,
                                         GomEventListener *listener,
                                         gboolean          use_capture)
{
    GSList *li;

    li = find_entry (list->listeners, namespace_uri, type, listener, use_capture);
    if (li) {
        entry_free (li->data);
        list->listeners = g_slist_delete_link (list->listeners, li);
    }
}

gboolean
gom_listener_list_has_event_listener (GomListenerList *list,
                                      const char      *namespace_uri,
                                      const char      *type)
{
    return find_entry (list->listeners, namespace_uri, type, NULL, TRUE) ||
        find_entry (list->listeners, namespace_uri, type, NULL, FALSE);
}

gboolean
gom_listener_list_will_trigger (GomListenerList *list,
                                GomEventTarget  *target,
                                const char      *namespace_uri,
                                const char      *type)
{
    GomEventTarget *parent = NULL;
    gboolean ret;
    if (gom_listener_list_has_event_listener (list, namespace_uri, type)) {
        return TRUE;
    }
    g_object_get (target, "parent", &parent, NULL);
    if (!parent) {
        return FALSE;
    }
    ret = gom_event_target_will_trigger_ns (parent, namespace_uri, type);
    g_object_unref (parent);
    return ret;
}

void
gom_listener_list_dispatch_listeners (GomListenerList *list,
                                      GomEventTarget  *current_target,
                                      GomEvent        *evt,
                                      const char      *namespace_uri,
                                      const char      *type_name,
                                      GomPhaseType     phase)
{
    ListenerEntry *entry;
    GSList *li;

    if (!GOM_IS_CUSTOM_EVENT (evt)) {
        g_warning (G_STRLOC": cannot dispatch an event that is not a CustomEvent");
    } else {
        for (li = list->listeners;
             (li = find_entry (li, namespace_uri, type_name, NULL, phase == GOM_CAPTURING_PHASE));
             li = li->next) {
            entry = li->data;
            gom_custom_event_set_dispatch_state (GOM_CUSTOM_EVENT (evt), current_target, phase);
            gom_event_listener_handle_event (entry->listener, evt);
            if (gom_custom_event_is_immediate_propagation_stopped (GOM_CUSTOM_EVENT (evt))) {
                break;
            }
        }
    }
}

gboolean
gom_listener_list_dispatch_event (GomEventTarget  *target,
                                  GomEvent        *evt,
                                  GError         **error)
{
    char *namespace_uri, *type_name;
    gboolean bubbles;
    GList  *parents, *li;
    GomNode *node;
    
    if (!GOM_IS_EVT (evt) || !GOM_IS_CUSTOM_EVENT (evt)) {
        g_warning ("I cannot work with this Event.");
        return FALSE;
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
        /* g_object_get() refs the object */
        parents = g_list_prepend (parents, node);
    }
    
    /* CAPTURING_PHASE */
    for (li = parents; li; li = li->next) {
        if (!GOM_IS_NODE_INTERNAL (li->data)) {
            g_warning (G_STRLOC": cannot dispatch event to %s %p",
                       g_type_name (G_TYPE_FROM_INSTANCE (li->data)), li->data);
            continue;
        }
        gom_node_internal_dispatch_listeners (GOM_NODE_INTERNAL (li->data),
                                              evt, namespace_uri, type_name, GOM_CAPTURING_PHASE);
        if (gom_custom_event_is_propagation_stopped (GOM_CUSTOM_EVENT (evt)) ||
            gom_custom_event_is_immediate_propagation_stopped (GOM_CUSTOM_EVENT (evt))) {
            goto dispatch_out;
        }
    }

    /* AT_TARGET */
    gom_node_internal_dispatch_listeners (GOM_NODE_INTERNAL (target),
                                          evt, namespace_uri, type_name, GOM_AT_TARGET);
    if (gom_custom_event_is_propagation_stopped (GOM_CUSTOM_EVENT (evt)) ||
        gom_custom_event_is_immediate_propagation_stopped (GOM_CUSTOM_EVENT (evt))) {
        goto dispatch_out;
    }

    /* BUBBLING_PHASE */
    if (bubbles) {
        for (li = g_list_last (parents); li; li = li->prev) {
            if (!GOM_IS_NODE_INTERNAL (li->data)) {
                g_warning (G_STRLOC": cannot dispatch event to %s %p",
                           g_type_name (G_TYPE_FROM_INSTANCE (li->data)), li->data);
                continue;
        }
            gom_node_internal_dispatch_listeners (GOM_NODE_INTERNAL (li->data),
                                                  evt, namespace_uri, type_name, GOM_BUBBLING_PHASE);
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

GomListenerList *
gom_listener_list_new (void)
{
    return g_new0 (GomListenerList, 1);
}

void
gom_listener_list_free (GomListenerList *list)
{
    if (!list) {
        return;
    }
    if (list->listeners) {
        g_slist_foreach (list->listeners, (GFunc)entry_free, NULL);
        g_slist_free (list->listeners);
        list->listeners = NULL;
    }
    g_free (list);
}
