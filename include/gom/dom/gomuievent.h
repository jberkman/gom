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
#ifndef GOM_UI_EVENT_H
#define GOM_UI_EVENT_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomUIEvent          GomUIEvent; /*dummy object*/
typedef struct _GomUIEventInterface GomUIEventInterface;

G_END_DECLS

#include <gom/dom/gomabstractview.h>

G_BEGIN_DECLS

#define GOM_TYPE_UI_EVENT             (gom_ui_event_get_type ())
#define GOM_UI_EVENT(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_UI_EVENT, GomUIEvent))
#define GOM_IS_UI_EVENT(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_UI_EVENT))
#define GOM_UI_EVENT_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_UI_EVENT, GomUIEventInterface))

struct _GomUIEventInterface {
    GTypeInterface parent;

    void (*init_ui_event) (GomUIEvent      *evt,
                           const char      *type_arg,
                           gboolean         can_bubble_arg,
                           gboolean         cancelable_arg,
                           GomAbstractView *view_arg,
                           long             detail_arg);
};

GType gom_ui_event_get_type (void);

void  gom_ui_event_init_ui_event (GomUIEvent      *evt,
                                  const char      *type_arg,
                                  gboolean         can_bubble_arg,
                                  gboolean         cancelable_arg,
                                  GomAbstractView *view_arg,
                                  long             detail_arg);

G_END_DECLS

#endif /* GOM_UI_EVENT_H */