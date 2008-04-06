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
#ifndef GOM_LISTENER_LIST_H
#define GOM_LISTENER_LIST_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomListenerList  GomListenerList;

G_END_DECLS

#include <gom/dom/gomeventtarget.h>

G_BEGIN_DECLS

GomListenerList *gom_listener_list_new  (void);
void             gom_listener_list_free (GomListenerList *list);

void     gom_listener_list_dispatch_listeners (GomListenerList *list,
                                               GomEventTarget  *current_target,
                                               GomEvent        *evt,
                                               const char      *namespace_uri,
                                               const char      *type_name,
                                               GomPhaseType     phase);

gboolean gom_listener_list_dispatch_event (GomEventTarget  *target,
                                           GomEvent        *event,
                                           GError         **error);

void     gom_listener_list_add_event_listener (GomListenerList  *list,
                                               const char       *namespace_uri,
                                               const char       *type,
                                               GomEventListener *listener,
                                               gboolean          use_capture,
                                               GObject          *evt_group);

void     gom_listener_list_remove_event_listener (GomListenerList  *list,
                                                  const char       *namespace_uri,
                                                  const char       *type,
                                                  GomEventListener *listener,
                                                  gboolean          use_capture);

gboolean gom_listener_list_has_event_listener (GomListenerList *list,
                                               const char      *namespace_uri,
                                               const char      *type);

gboolean gom_listener_list_will_trigger (GomListenerList *list,
                                         GomEventTarget  *target,
                                         const char      *namespace_uri,
                                         const char      *type);

G_END_DECLS

#endif /* GOM_LISTENER_LIST_H */
