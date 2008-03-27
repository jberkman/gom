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
#ifndef GOM_MOUSE_EVENT_H
#define GOM_MOUSE_EVENT_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomMouseEvent          GomMouseEvent; /*dummy object*/
typedef struct _GomMouseEventInterface GomMouseEventInterface;

G_END_DECLS

#include <gom/dom/gomabstractview.h>
#include <gom/dom/gomeventtarget.h>

G_BEGIN_DECLS

#define GOM_TYPE_MOUSE_EVENT             (gom_mouse_event_get_type ())
#define GOM_MOUSE_EVENT(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_MOUSE_EVENT, GomMouseEvent))
#define GOM_IS_MOUSE_EVENT(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_MOUSE_EVENT))
#define GOM_MOUSE_EVENT_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_MOUSE_EVENT, GomMouseEventInterface))

struct _GomMouseEventInterface {
    GTypeInterface parent;

    void (*init_mouse_event) (GomMouseEvent *evt,
                              const char      *type_arg,
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
                              GomEventTarget  *related_target_arg);

    gboolean (*get_modifier_state) (GomMouseEvent *evt,
                                    const char    *key_identifier_arg);

    void (*init_mouse_event_ns) (GomMouseEvent   *evt,
                                 const char      *namespace_uri,
                                 const char      *type_arg,
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
                                 GomEventTarget  *related_target_arg);
};

GType gom_mouse_event_get_type (void);

void  gom_mouse_event_init_mouse_event (GomMouseEvent *evt,
                                        const char      *type_arg,
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
                                        GomEventTarget  *related_target_arg);

gboolean gom_mouse_event_get_modifier_state (GomMouseEvent *evt,
                                             const char    *key_identifier_arg);

void  gom_mouse_event_init_mouse_event_ns (GomMouseEvent   *evt,
                                           const char      *namespace_uri,
                                           const char      *type_arg,
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
                                           GomEventTarget  *related_target_arg);

G_END_DECLS

#endif /* GOM_MOUSE_EVENT_H */
